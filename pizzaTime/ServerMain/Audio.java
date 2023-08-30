import java.applet.*;
import java.io.File;
import java.net.*;
import javax.sound.sampled.AudioInputStream; 
import javax.sound.sampled.AudioSystem; 
import javax.sound.sampled.Clip; 
import javax.sound.sampled.LineUnavailableException; 
import javax.sound.sampled.UnsupportedAudioFileException; 
public class Audio{
    
    /**
     * plays and loops the music on the background.
     */
    public static void playAudio() {
        System.out.println("");
        String contin = "y";
        try {
            AudioInputStream in = AudioSystem.getAudioInputStream (new File("music.wav"));
            Clip clip = AudioSystem.getClip();
            clip.open(in); 

            clip.loop(Clip.LOOP_CONTINUOUSLY); 
            if (contin.equals("y"))
            {
                clip.start();
            }
            else 
            {   clip.stop();
            }
            //AudioClip clip = Applet.newAudioClip(new File("music.mp3"));
            // clip.play();

        } 
        catch (UnsupportedAudioFileException murle) {
            System.out.println(murle);
        }
        catch (Exception e)
        {
            System.out.println(e);
        }
    }
}