package ICE_Logic;

import  org.json.simple.*;

public class Character {

    //************************* Class Fields ****************************//
    private int posX;//Determines character X position
    private int posY;//Determines character y position
    private int score;//Determines character score
    private int lives;//Determines character lives

    //************************ Class Constructor ************************//

    //Constructor, needs initial position of character, score and lives
    //In case of player:
    //score -> 0
    //lives -> 3
    //In case of enemy:
    //lives -> 1
    //score -> determined in documentation

    public Character(int posX,int posY,int score, int lives){
        this.posX=posX;
        this.posY=posY;
        this.score=score;
        this.lives=lives;
    }

    //********************** Setters and Getters ************************//

    public int getPosX() {
        return posX;
    }

    public void setPosX(int posX) {
        this.posX = posX;
    }

    public int getPosY() {
        return posY;
    }

    public void setPosY(int posY) {
        this.posY = posY;
    }

    public int getScore() {
        return score;
    }

    public void setScore(int score) {
        this.score = score;
    }

    public int getLives() {
        return lives;
    }

    public void setLives(int lives) {
        this.lives = lives;
    }


    //************************** Update Method **************************//

    //Receive a json object to update data values

    public void update(JSONObject json){

        this.posX= (int)((long) json.get("posX"));
        this.posY=(int) ((long)json.get("posY"));
        this.score=(int) ((long)json.get("score"));
        this.lives=(int) ((long)json.get("lives"));
    }

    //************************** Refresh Method *************************//

    //Returns character data as a json object to be sent to the client

    public JSONObject refresh(){
        JSONObject json=new JSONObject ();
        json.put("posX",this.posX);
        json.put("posY", this.posY);
        json.put("score",this.score);
        json.put("lives",this.lives);
        return json;
    }

}
