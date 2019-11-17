package ICE_Logic;

import org.json.simple.JSONArray;
import org.json.simple.JSONObject;

public class SpawnManager {

    int enemyCounter=0;
    JSONArray enemyArray=new JSONArray();

    //Enemies
    public JSONObject createEnemy(){

        Character enemy=new Character(0,0,0,1);
        JSONObject json=new JSONObject();

        json.put("id",enemyCounter);
        json.put("posX",enemy.getPosX());
        json.put("posY", enemy.getPosY());
        json.put("score",enemy.getScore());
        json.put("lives",enemy.getLives());

        enemyCounter+=1;

        return json;

    }

    public void destroyEnemy(){

    }



}
