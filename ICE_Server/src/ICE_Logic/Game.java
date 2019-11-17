package ICE_Logic;


import org.json.simple.*;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class Game {

    Character Popo=new Character(48,392,0,3);
    //Character Nana=new Character(592,392,0,3);
    //SpawnManager manager=new SpawnManager();

    //public Game(int players){
    //
    //}

    public void update(String input){
        try {

            JSONParser parser = new JSONParser();
            JSONObject  json = (JSONObject ) parser.parse(input);
            
            if((int)((long) json.get("id"))==1){
                this.Popo.update((JSONObject)json.get("Popo"));
            }// else if ((int)((long) json.get("id"))==2){
              //  this.Nana.update((JSONObject) json.get("Nana"));
            //}

        }catch (ParseException e){
            e.printStackTrace();
        }


    }




    public String refresh(){
        //JSONObject json=new JSONObject ();
        //json.put("Popo",this.Popo.refresh());
        //json.put("Nana",this.Nana.refresh());
        //return json.toJSONString();
        return this.Popo.refresh().toJSONString();

    }

}
