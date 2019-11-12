package ICE_Server;

import java.io.*;
import java.net.Socket;
import java.util.Scanner;

public class Client {

    private int port=9090;
    private String hostName="localhost";

    public void initClient(){

        try{
            //Creates socket connection
            Socket socket=new Socket(this.hostName,this.port);
            //Output
            OutputStream output = socket.getOutputStream();
            PrintWriter writer=new PrintWriter(output,true);
            //Reads console input
            //Console console= System.console();
            Scanner scanner=new Scanner(System.in);
            String outputData;
            do{
                outputData=scanner.nextLine();
                writer.println(outputData);

                InputStream input=socket.getInputStream();
                BufferedReader reader= new BufferedReader(new InputStreamReader(input));
                String inputData=reader.readLine();
                System.out.print(inputData+"\n");


            }while (!output.equals("bye"));

            //socket.close();

        }catch (IOException e){
            e.printStackTrace();
        }
    }

    public static void main(String[] args){
        Client client=new Client();
        client.initClient();
    }
}
