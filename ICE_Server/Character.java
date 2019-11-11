package ICE_Server;

import com.github.cliftonlabs.json_simple.JsonObject;

public class Character {

    private int posX;
    private int posY;
    private int score;
    private int lives;

    public void Character(){

        this.posX=0;
        this.posY=0;
        this.score=0;
        this.lives=3;
    }

    public void update(JsonObject json){

        this.posX= (int) json.get("posX");
        this.posY=(int) json.get("posY");
        this.score=(int) json.get("score");
        this.lives=(int) json.get(lives);
    }

    public JsonObject refresh(){
        JsonObject json=new JsonObject();
        json.put("posX",this.posX);
        json.put("posY", this.posY);
        json.put("score",this.score);
        json.put("lives",this.lives);
        return json;
    }

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
}
