import java.awt.Image;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;


public class Main {
	static boolean programRun = true;
	
	public static boolean verifyNrArguments(String []args) throws InterruptedException{
		if (args.length != 4){
			System.err.print("Verificare numar de argumente: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
		else {
			System.out.print("Verificare numar de argumente: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
	}
	
	public static boolean verifyExec(String binName) throws InterruptedException{
		File bin = new File(binName);
		if (bin.exists()){
			System.out.print("Verificare existenta executabil: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare existenta executabil: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyExecRights(String binName) throws InterruptedException{
		File bin = new File(binName);
		if (bin.canExecute()){
			System.out.print("Verificare drept de executie executabil: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare drept de executie executabil: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyInputDir(File inputDir) throws InterruptedException{
		if (inputDir.exists()){
			System.out.print("Verificare existenta director de input: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare existenta director de input: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyInputDirRights(File inputDir) throws InterruptedException{
		if (inputDir.canRead()){
			System.out.print("Verificare drepturi de citire din directorul de input: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare drepturi de citire din directorul de input: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyOutputTifDir(File outputTifDir) throws InterruptedException{
		if (outputTifDir.exists()){
			System.out.print("Verificare existenta director de output pentru tif-uri: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare existenta director de output pentru tif-uri: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyOutputTifDirRights(File outputTifDir) throws InterruptedException{
		if (outputTifDir.canWrite()){
			System.out.print("Verificare drepturi de scriere in directorul de output pentru tif-uri: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare drepturi de scriere in directorul de output pentru tif-uri: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyOutputConfDir(File outputConfDir) throws InterruptedException{
		if (outputConfDir.exists()){
			System.out.print("Verificare existenta director de output pentru confidente: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare existenta director de output pentru confidente: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyOutputConfDirRights(File outputConfDir) throws InterruptedException{
		if (outputConfDir.canWrite()){
			System.out.print("Verificare drepturi de scriere in directorul de output pentru confidente: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("Verificare drepturi de scriere in directorul de output pentru confidente: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyInputNrOfFiles(File []inputImages) throws InterruptedException{
		if (inputImages.length == 0){
			System.err.print("Numar de fisiere in directorul de intrare - ");
			System.err.println("0: Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
		else{
			System.out.print("Numar de fisiere in directorul de intrare - ");
			System.out.println(inputImages.length + ": Succes");
			System.out.flush();
			return true;
		}
	}
	
	public static boolean verifyImageType(File inputImage) throws InterruptedException{
		String ext = inputImage.getName().split("\\.")[1];
		if (ext.equals("png") || ext.equals("tif") || ext.equals("jpg") || ext.equals("bmp") || ext.equals("gif")){
			System.out.print("..........Verificare fisier tip imagine: ");
	    	System.out.println("Succes");
	    	System.out.flush();
	    	return true;
		}
		try {
		    Image image =ImageIO.read(inputImage);
		    if (image == null) {
		    	System.err.print("..........Verificare fisier tip imagine: ");
		        System.err.println("Esec");
		        System.err.flush();
		        Thread.sleep(10);
		        return false;
		    }
		    else {
		    	System.out.print("..........Verificare fisier tip imagine: ");
		    	System.out.println("Succes");
		    	System.out.flush();
		    	return true;
		    }
		} catch(IOException ex) {
			ex.printStackTrace();
			return false;
		}
	}
	
	public static boolean verifyOutputTifFile(File outputTifFile) throws InterruptedException{
		if (outputTifFile.exists()){
			System.out.print("..........Verificare existenta fisier de output binarizat: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("..........Verificare existenta fisier de output binarizat: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static boolean verifyOutputConfFile(File outputConfFile) throws InterruptedException{
		if (outputConfFile.exists()){
			System.out.print("..........Verificare existenta fisier de confidente: ");
			System.out.println("Succes");
			System.out.flush();
			return true;
		}
		else{
			System.err.print("..........Verificare existenta fisier de confidente: ");
			System.err.println("Esec");
			System.err.flush();
			Thread.sleep(10);
			return false;
		}
	}
	
	public static void main(String[] args) throws InterruptedException {
		programRun = programRun & verifyNrArguments(args);
		
		if (!programRun){
			System.err.println("Executia programului a esuat!");
			return;
		}
		
		File inputDir = new File(args[1]);
		File outputTifDir = new File(args[2]);
		File outputConfDir = new File(args[3]);
		
		programRun = verifyExec(args[0]) & verifyExecRights(args[0]) 
				& verifyInputDir(inputDir) & verifyInputDirRights(inputDir)
				& verifyOutputTifDir(outputTifDir) & verifyOutputTifDirRights(outputTifDir)
				& verifyOutputConfDir(outputConfDir) & verifyOutputConfDirRights(outputConfDir);
		
		if (!programRun){
			System.err.println("Executia programului a esuat!");
			return;
		}
		
		String bin = "\"" + args[0] + "\"";
		String inputDirPath = args[1] + File.separator;
		String outputTiffDirPath = args[2] + File.separator;
		String outputConfDirPath = args[3] + File.separator;
		
		File []inputImages = inputDir.listFiles();
		String []inputImagesName = inputDir.list();
		
		programRun &= verifyInputNrOfFiles(inputImages);
		
		if (!programRun){
			System.err.println("Executia programului a esuat! Nu exista fisiere de input in directorul ales!");
			return;
		}
		
		for (int i = 0; i < inputImages.length; i++){
			System.out.println(".....Procesare fisier " + inputImagesName[i]);
			System.out.flush();
			boolean imageProcess = verifyImageType(inputImages[i]);
								
			if (imageProcess){
				String input = inputImagesName[i];
				String outTif = input.split("\\.")[0] + "_bin.tif";
				String outConf = input.split("\\.")[0] + "_conf.tif";
				String command = bin + " " + inputDirPath + inputImagesName[i] + " " 
						+ outputTiffDirPath + outTif + " " + outputConfDirPath + outConf;
				try {
					long begin = System.currentTimeMillis();
					Process p = Runtime.getRuntime().exec(command);
					p.waitFor();
					long timeProcess = (System.currentTimeMillis() - begin)/1000;

					if (timeProcess < 300){
						System.out.print("..........Timp de procesare: ");
						System.out.println(timeProcess + " secunde");
						System.out.flush();
					}
					else{
						System.err.print("..........Timp de procesare: ");
						System.err.println(timeProcess + " secunde");
						System.err.flush();
						Thread.sleep(10);
					}
					imageProcess &= verifyOutputTifFile(new File(outputTiffDirPath + outTif));
					imageProcess &= verifyOutputConfFile(new File(outputConfDirPath + outConf));
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
			
			if (!imageProcess){
				System.err.print(".....Procesare fisier " + inputImagesName[i] + ": ");
				System.err.println("Esec");
				System.err.flush();
				Thread.sleep(10);
			}
			else{
				System.out.print(".....Procesare fisier " + inputImagesName[i] + ": ");
				System.out.println("Succes");
				System.out.flush();
			}
		}
		System.out.println("Executia programului s-a incheiat cu succes");
		System.out.flush();

	}
}
