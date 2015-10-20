import java.applet.Applet;
import java.applet.AppletContext;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.Robot;
import java.awt.Toolkit;
import java.awt.image.BufferedImage;
import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.net.MalformedURLException;
import java.net.URL;
import javax.imageio.ImageIO;
import org.apache.commons.codec.binary.Base64;

public class Screenshot extends Applet
{
  String modulePath;
  byte[] imageInByte;
  static String encoded;
  public int ready = 0;
  public boolean running = true;
  public int currentWidth;
  public int currentHeight;

  public void doit(int paramInt1, int paramInt2)
  {
    this.currentWidth = paramInt1;
    this.currentHeight = paramInt2;
    this.ready = 1;
  }

  public void destroy() {
    this.running = false;
  }

  public void start() {
    this.modulePath = getParameter("modulePath");
    while (this.running)
    {
      if (this.ready == 1)
      {
        this.running = false;
        System.out.println("now ready and calling sceenshot");
        takeScreenshot();
      }
      try {
        Thread.sleep(100L);
      }
      catch (Exception localException) {
        System.out.println(localException.toString());
      }
    }
  }

  public BufferedImage cropImage(BufferedImage paramBufferedImage, Rectangle paramRectangle) {
    BufferedImage localBufferedImage = paramBufferedImage.getSubimage(paramRectangle.x, paramRectangle.y, paramRectangle.width, paramRectangle.height);
    return localBufferedImage;
  }

  public void takeScreenshot() {
    try {
      Robot localRobot = new Robot();
      Rectangle localRectangle1 = new Rectangle(Toolkit.getDefaultToolkit().getScreenSize());
			System.out.println("calling start");
      BufferedImage localBufferedImage = localRobot.createScreenCapture(localRectangle1);
			System.out.println("done");
      Point localPoint = getLocationOnScreen();
      System.out.println(localPoint.x + " " + localPoint.y + " " + this.currentWidth + " " + this.currentHeight);
      Rectangle localRectangle2 = new Rectangle(localPoint.x, localPoint.y, this.currentWidth, this.currentHeight);
      localBufferedImage = cropImage(localBufferedImage, localRectangle2);
      ByteArrayOutputStream localByteArrayOutputStream = new ByteArrayOutputStream();
      ImageIO.write(localBufferedImage, "png", localByteArrayOutputStream);
      localByteArrayOutputStream.flush();
      this.imageInByte = localByteArrayOutputStream.toByteArray();
      localByteArrayOutputStream.close();
      new Base64(); encoded = Base64.encodeBase64String(this.imageInByte);
      try
      {
        getAppletContext().showDocument(new URL("javascript:feedbackReloaded.saveScreenshot();"));
      }
      catch (MalformedURLException localMalformedURLException) {
        System.out.println(localMalformedURLException.toString());
      }

    }
    catch (Exception localException)
    {
      System.out.println(localException.toString());
    }
  }

  public String getScreenshotData()
  {
    return encoded;
  }
}