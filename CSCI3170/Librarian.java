import java.text.SimpleDateFormat;
import java.util.Scanner;
import java.util.Date;
import java.sql.*;

class Librarian {
    private static Connection con = null;
    private static String title = "Operations for librarian menu";
    private static String[] optionList = {
        "Book Borrowing",
        "Book Returning",
        "List all un-returned book copies which are checked-out within a period",
        "Return to the main menu"
    };

    public static void main (Connection con) {
        Librarian.con = con;
        while (true) {
            int option = CSCI3170Proj.getMenuChoice(title, optionList);
            try {
                switch(option) {
                    case 1:
                        borrowBook();
                        break;
                    case 2:
                        returnBook();
                        break;
                    case 3:
                        listUnreturnedBook();
                        break;
                    case 4:
                        return;
                }
            }
            catch (Exception e) {
                System.out.println(e);
            }
        }
    }

    private static void borrowBook () {
        Scanner scanner = new Scanner(System.in);
        
        // get user id and get max borrow limit
        int maxBook;
        System.out.print("Enter The User ID: ");
        String inpUserID = scanner.nextLine();
        try {
            String query = "SELECT user_category.max_book FROM libuser, user_category WHERE libuser.libuid=? AND libuser.ucid=user_category.ucid;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpUserID);
            ResultSet resultSet = stmt.executeQuery();
            
            if (!resultSet.isBeforeFirst()) {
                System.out.println("[Error]: User not found!");
                return;
            }

            resultSet.next();
            maxBook = resultSet.getInt(1);
        }
        catch (Exception e) {
            System.out.println("[Error]: User not found!");
            return;
        }

        // get call num
        System.out.print("Enter the Call Number: ");
        String inpCallNum = scanner.nextLine();
        try {
            String query = "SELECT COUNT(*) FROM book WHERE callnum = ?;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            
            if (resultSet.getInt(1) == 0) {
                System.out.println("[Error]: Book not found!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: Book not found!");
            return;
        }

        // get copy num
        System.out.print("Enter The Copy Number: ");
        int inpCopyNum;
        try {
            inpCopyNum = Integer.parseInt(scanner.nextLine());
            if(inpCopyNum >= 10 || inpCopyNum <= 0) throw new Exception();

            String query = "SELECT COUNT(*) FROM book_copy WHERE callnum = ? AND copynum = ?;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            stmt.setInt(2, inpCopyNum);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            
            if (resultSet.getInt(1) == 0) {
                System.out.println("[Error]: Copy not found!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: Copy not found!");
            return;
        }

        // count user borrowed book
        try {
            String query = "SELECT COUNT(*) FROM borrow WHERE libuid=? AND return_date IS NULL;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpUserID);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            if(maxBook <= resultSet.getInt(1)) {
                System.out.println("[Error]: You exceed the limit of borrow quota!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: Database connection lost!");
            return;
        }

        // check book availability
        try {
            String query = "SELECT COUNT(*) FROM borrow WHERE callnum=? AND copynum=? AND return_date IS NULL;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            stmt.setInt(2, inpCopyNum);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            if(resultSet.getInt(1) > 0) {
                System.out.println("[Error]: The book is already borrowed!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: Database connection lost!");
            return;
        }

        // add borrow record
        try {
            String query = "INSERT INTO borrow VALUES (?, ?, ?, NOW(), NULL);";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            stmt.setInt(2, inpCopyNum);
            stmt.setString(3, inpUserID);
            stmt.executeUpdate();
            System.out.println("Book borrowing performed successfully.");
        }
        catch (SQLException e) {
            if(e.getErrorCode() == 1062) {
                System.out.println("[Error]: You are not allowed to borrow this book copy until tomorrow!");
                return;
            }
            System.out.println("[Error]: Failed to borrow the book!");
        }
    }

    private static void returnBook () {
        Scanner scanner = new Scanner(System.in);

        // read user id
        System.out.print("Enter The User ID: ");
        String inpUserID = scanner.nextLine();
        try {
            String query = "SELECT COUNT(*) FROM libuser WHERE libuid=?;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpUserID);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            
            if (resultSet.getInt(1) == 0) {
                System.out.println("[Error]: Book not found!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: User not found!");
            return;
        }

        // get call num, borrowed times, book rating
        int tborrowed;
        double bookRating;
        System.out.print("Enter the Call Number: ");
        String inpCallNum = scanner.nextLine();
        try {
            String query = "SELECT tborrowed, rating FROM book WHERE callnum = ?;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            ResultSet resultSet = stmt.executeQuery();
            
            if(!resultSet.isBeforeFirst()) {
                System.out.println("[Error]: Book not found!");
                return;
            }
            
            resultSet.next();
            tborrowed = resultSet.getInt(1);
            bookRating = resultSet.getDouble(2);
        }
        catch (Exception e) {
            System.out.println("[Error]: Book not found!");
            return;
        }

        // get copy number
        System.out.print("Enter The Copy Number: ");
        int inpCopyNum;
        try {
            inpCopyNum = Integer.parseInt(scanner.nextLine());
            if(inpCopyNum >= 10 || inpCopyNum <= 0) throw new Exception();

            String query = "SELECT COUNT(*) FROM book_copy WHERE callnum = ? AND copynum = ?;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            stmt.setInt(2, inpCopyNum);
            ResultSet resultSet = stmt.executeQuery();
            resultSet.next();
            
            if (resultSet.getInt(1) == 0) {
                System.out.println("[Error]: Copy not found!");
                return;
            }
        }
        catch (Exception e) {
            System.out.println("[Error]: Copy not found!");
            return;
        }

        // get valid rating
        double userRating;
        while (true) {
            try {
                System.out.print("Enter Your Rating of the Book: ");
                userRating = Double.parseDouble(scanner.nextLine());
                if(userRating > 10.0 || userRating < 0.0) throw new Exception();
                break;
            }
            catch (Exception e) {
                System.out.println("[Error]: Invalid input! Please input rating in (1-10).");
            }
        }

        // run transaction for return process
        try {
            con.setAutoCommit(false);

            // update the borrow record
            String query = "UPDATE borrow SET return_date=NOW() WHERE callNum=? AND copynum=? AND libuid=? AND return_date IS NULL;";
            PreparedStatement stmt = con.prepareStatement(query);
            stmt.setString(1, inpCallNum);
            stmt.setInt(2, inpCopyNum);
            stmt.setString(3, inpUserID);
            int updatedRow = stmt.executeUpdate();
            if(updatedRow == 0) {
                System.out.println("[Error]: You have not borrowed this book!");
                con.rollback();
                con.setAutoCommit(true);
                return;
            }
            
            // update the new rating and borrowed times
            double newrating = (bookRating * tborrowed + userRating)/(tborrowed + 1);
            query = "UPDATE book SET rating=?, tborrowed=? WHERE callnum=?;";
            stmt = con.prepareStatement(query);
            stmt.setDouble(1, newrating);
            stmt.setInt(2, tborrowed + 1);
            stmt.setString(3, inpCallNum);
            stmt.executeUpdate();

            con.commit();
            con.setAutoCommit(true);
            System.out.println("Book returning performed successfully.");
        }
        catch (Exception e) {
            try {
                con.rollback();
                con.setAutoCommit(true);
            }
            catch (Exception ex) {}
            System.out.print("[Error]: Failed to return the book!");
        }
    }

    private static void listUnreturnedBook () {
        Scanner scanner = new Scanner(System.in);
        Date startDate, endDate;

        // read valid dates
        while(true) {
            System.out.print("Type in the starting date [dd/mm/yyyy]: ");
            String start = scanner.nextLine();
            System.out.print("Type in the ending date [dd/mm/yyyy]: ");
            String end = scanner.nextLine();
            try {
                startDate = new SimpleDateFormat("dd/MM/yyyy").parse(start);
                endDate = new SimpleDateFormat("dd/MM/yyyy").parse(end);
                break;
            }
            catch (Exception e) {
                System.out.println("[Error]: Invalid date format!");
            }
        }

        // list all borrow record within the period
        try {
            String psql = "SELECT * FROM borrow WHERE return_date IS NULL AND checkout >= ? AND checkout <= ? ORDER BY checkout DESC;";
            PreparedStatement pstmt = con.prepareStatement(psql);
            pstmt.setDate(1, new java.sql.Date(startDate.getTime()));
            pstmt.setDate(2, new java.sql.Date(endDate.getTime()));
            ResultSet records = pstmt.executeQuery();

            if(!records.isBeforeFirst()) {
                System.out.println("No record within the period!");
                return;
            }

            System.out.println("List of UnReturned Book:");
            String format = String.format("|%%-%ds|%%-%ds|%%-%ds|%%-%ds|\n", 10, 8, 8, 10);
            System.out.format(format, "Lib UID", "Call Num", "Copy Num", "Checkout");
            while(records.next()) {
                String callNum = records.getString(1);
                int copyNum = records.getInt(2);
                String libUID = records.getString(3);
                String checkout = records.getDate(4).toString();
                System.out.format(format, libUID, callNum, copyNum, checkout);
            }
            System.out.println("End of Query");
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }
}
