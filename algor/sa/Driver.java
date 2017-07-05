import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.*;
public class Driver {
public static void main(String[] args) throws FileNotFoundException
   {
      EditDistance box = new EditDistance();
      box.editDistance(args[0]);
   }
}