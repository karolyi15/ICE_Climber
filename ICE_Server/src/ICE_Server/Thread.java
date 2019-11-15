package ICE_Server;

import java.io.*;
import java.net.Socket;

public class Thread {
    private Socket socket;
    private boolean running=true;

    public Thread(Socket socket) {

        this.socket=socket;
    }

    public void start(){
        try {
            //************** Read data to client ****************//

            //Read data from the client (read to byte array)
            InputStream input = socket.getInputStream();
            //Set byte array to string
            //InputStreamReader ireader=new InputStreamReader(input);
            BufferedReader reader = new BufferedReader(new InputStreamReader(input),1024);

            //************** Send data to client ***************//

            //Write data as a byte array
            OutputStream output = socket.getOutputStream();
            //Converts byte array to text format
            PrintWriter writer = new PrintWriter(output, true);
            //writer.println("Hellow World");

            String inputData;
            char test[]=new char[1024];
            Game game=new Game();


            do {

                //Input from Client
                reader.read(test,0,1024);
                System.out.println(String.valueOf(test).trim());
                game.update(String.valueOf(test).trim());



                //Output to Client
                writer.println(game.refresh());
                this.running=false;

            } while (running);


            //*********** Close client connection *************//
            //ICE_Server.Server close socket connection but still work for other sockets
            socket.close();
        }catch (IOException e) {
            e.printStackTrace();
        }
    }


}





