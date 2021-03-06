package extra;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Sockets {

	/**
	 * @param args
	 * @throws IOException
	 * @throws UnknownHostException
	 */
	public static void main(String[] args) throws UnknownHostException,
			IOException {
		System.out.print("Requesting... " + args[0] + "\n");
		Socket sock = new Socket(args[0], 80);
		try {
			BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream()));
			String request = "GET /?gws_rd=cr HTTP/1.1\r\n"
					+ "Host: " + args[0] + "\r\n" + "Accept: */*\r\n"
					+ "User-Agent: Java\r\n" + "Connection: close\r\n" + "\r\n";
			bw.write(request);
			bw.flush();

			BufferedReader br = new BufferedReader(new InputStreamReader(sock.getInputStream()));
			String read = null;
			while ((read = br.readLine()) != null) {
				System.out.println(read);
			}
			bw.close();
		} finally {
			sock.close();
		}
	}

}