// javac CSCI3170Proj.java Administrator.java LibraryUser.java Librarian.java
// java -cp .:connector.jar CSCI3170Proj Administrator.class LibraryUser.class Librarian.class
import java.util.Scanner;
import java.sql.*;

class CSCI3170Proj {
    private static final String dbAddress = "jdbc:mysql://projgw.cse.cuhk.edu.hk:2633/db22?allowMultiQueries=true";
    private static final String dbUsername = "Group22";
    private static final String dbPassword = "CSCI3170";
    private static Connection con = null;
    private static String title = "Main menu";
    private static String[] optionList = {
        "Operations for Administrator",
        "Operations for Library User",
        "Operations for Librarian",
        "Exit this program"
    };

    public static void main (String[] args) {
        try {
            Class.forName("com.mysql.jdbc.Driver");
            con = DriverManager.getConnection(dbAddress, dbUsername, dbPassword);
            
            System.out.println("Welcome to Library Inquiry System!");
            
            while(true) {
                int option = getMenuChoice(title, optionList);
                switch(option) {
                    case 1:
                        Administrator.main(con);
                        break;
                    case 2:
                        LibraryUser.main(con);
                        break;
                    case 3:
                        Librarian.main(con);
                        break;
                    case 4:
                        return;
                }
            }
        }
        catch (ClassNotFoundException e) {
            System.out.println("[Error]: Java MySQL DB Driver not found1");
            System.exit(0);
        }
        catch (Exception e) {
            System.out.println("[Error]: " + e.getMessage());
        }
    }

    public static int getMenuChoice (String title, String[] optionList) {
        Scanner scanner = new Scanner(System.in);
        int option = 0;

        // get valid option
        while(option < 1 || option > optionList.length) {
            System.out.println("\n-----" + title + "-----");
            System.out.println("What kinds of operations would you like to perform?");
            for(int i = 0; i < optionList.length; i++) {
                System.out.println((i + 1) + ". " + optionList[i]);
            }

            try {
                System.out.print("Enter Your Choice: ");
                String input = scanner.nextLine();
                option = Integer.parseInt(input);
                if(option < 1 || option > optionList.length) throw new Exception();
            }
            catch (Exception e) {
                System.out.println("[Erorr]: Invalid input!");
            }
        }
        
        return option;
    }
}