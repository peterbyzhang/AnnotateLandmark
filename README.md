- PURPOSE

     Add annotation to image, and save results in an xml file. This version adds pupils to the 68 point facial 
     landmarks, but it can be easily modified to fit other purposes.

     The landmarks are stored in an xml file, compitable to the format used by dlib. 
     This program is modified from taotao1233's work <http://blog.csdn.net/jinshengtao/article/details/42614091>.

- HOW TO RUN

     Run the program by specifying the path of the xml file as the first argument.

     The dataset folder contains an example dataset from dlib, and an exe file prebuilt for Windows 10 X64 PC. 
     You may test the program with the following command:

     .\dataset> Annotate training_with_face_landmarks

     The program will overwrite the xml, so please backup before running the program.

- DEPENDENCIES

     This program is dependent on OpenCV. 

     The first line in main() is the only thing that makes the program dependent on Windows. Basically it 
     guarantees the annotation works are saved if the user quits the program in the unrecommended way of 
     closing the command window.

     You should be able to make this program run in another OS if you delete or replace that line.

- BEHAVIOR

     The following instructions can also be found in the command window when the program is runned:

     The yellow landmark is the eye on the user's right hand side.
     The cyan landmark is the eye on the user's left hand side.

     MOUSE CONTROL:
     Click mouse to add eye points, always add yellow point before cyan.
     After the eye points are added, click mouse to move them.

     KEYBOARD CONTROL:
     D - next face
     A - previous face
     R - reload landmarks
     Q - save and quit
