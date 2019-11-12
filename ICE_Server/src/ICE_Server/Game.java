package ICE_Server;


import org.json.simple.*;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;

public class Game {

    Character Popo=new Character();

    public void update(String input){
        try {

            JSONParser parser = new JSONParser();
            JSONObject  json = (JSONObject ) parser.parse(input);
            this.Popo.update((JSONObject) json.get("Popo"));

        }catch (ParseException e){
            e.printStackTrace();
        }


    }



    public String refresh(){
        JSONObject json=new JSONObject ();
        json.put("Popo",this.Popo.refresh());
        return json.toJSONString();

    }

}
