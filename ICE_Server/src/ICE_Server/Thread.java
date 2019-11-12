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
            BufferedReader reader = new BufferedReader(new InputStreamReader(input));

            //************** Send data to client ***************//

            //Write data as a byte array
            OutputStream output = socket.getOutputStream();
            //Converts byte array to text format
            PrintWriter writer = new PrintWriter(output, true);
            //writer.println("Hellow World");

            String inputData;
            Game game=new Game();


            do {
                //Input from Client
                inputData = reader.readLine();
                System.out.print(inputData+"\n");
                game.update(inputData);

                //Output to Client
                writer.println(game.refresh()+"\n");
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





