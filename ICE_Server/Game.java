package ICE_Server;

import com.github.cliftonlabs.json_simple.*;

public class Game {
    Character Popo=new Character();

    public void update(){

    }

    public String refresh(){
        JsonObject json=new JsonObject();
        json.put("Popo",this.Popo.refresh());
        return json.toString();

    }

}
