import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Scanner;
import java.sql.*;

class LibraryUser {
    private static Connection con = null;
    private static String title = "Operations for library user menu";
    private static String[] optionList = {
        "Search for Books",
        "Show loan record of a user",
        "Return to the main menu"
    };

    public static void main (Connection con) {
        LibraryUser.con = con;
        
        while (true) {
            int option = CSCI3170Proj.getMenuChoice(title, optionList);
            try {
                switch(option) {
                    case 1:
                        searchforBook();
                        break;
                    case 2:
                        showLoanRecord();
                        break;
                    case 3:
                        return;
                }
            }
            catch (Exception e) {
                System.out.println(e);
            }
        }
    }

    private static void searchforBook () {
        Scanner scanner = new Scanner(System.in);

        // get valid criteria
        String input;
        int option = 0;
        while(option < 1 || option > 3) {
            System.out.println("Choose the search criterion:");
            System.out.println("1. call number");
            System.out.println("2. title");
            System.out.println("3. author");
            System.out.print("Choose the search criterion: ");

            try {
                input = scanner.nextLine();
                option = Integer.parseInt(input);
                if(option < 1 || option > optionList.length) throw new Exception();
            }
            catch (Exception e) {
                System.out.println("[Erorr]: Invalid input!");
            }
        }

        System.out.print("Type in the Search Keyword: ");
        input = scanner.nextLine();
        switch (option) {
            case 1:
                searchBookByCallNum(input);
                break;
            case 2:
                searchBookByTitle(input);
                break;
            case 3:
                searchBookByAuthor(input);
                break;
        }
    }

    private static void searchBookByCallNum (String callNum) {
        // check call num valid or not
        try {
            String psql = "SELECT callnum FROM book WHERE callnum=?;";
            PreparedStatement pstmt = con.prepareStatement(psql);
            pstmt.setString(1, callNum);
            ResultSet bookList = pstmt.executeQuery();

            if (!bookList.isBeforeFirst()) {
                System.out.println("No book were found!");
                return;
            }

            ArrayList<String> callNums = new ArrayList<String>();
            callNums.add(callNum);
            printBook(callNums);
        }
        catch(Exception e) {
            System.out.println(e);
        }
    }

    private static void searchBookByTitle (String bookTitle) {
        // get call num with matching title
        try {
            String psql = "SELECT callnum FROM book WHERE title LIKE ?;";
            bookTitle = bookTitle.replace("!", "!!").replace("%", "!%").replace("_", "!_").replace("[", "![");
            PreparedStatement pstmt = con.prepareStatement(psql);
            pstmt.setString(1, "%" + bookTitle + "%");
            ResultSet bookList = pstmt.executeQuery();

            if (!bookList.isBeforeFirst()) {
                System.out.println("No book were found!");
                return;
            }

            ArrayList<String> callNums = new ArrayList<String>();
            while (bookList.next()) {
                callNums.add(bookList.getString(1));
            }
            printBook(callNums);
        }
        catch(Exception e) {
            System.out.println(e);
        }
    }

    private static void searchBookByAuthor (String author) {
        // get call num with matching author
        try {
            String psql = "SELECT callnum FROM authorship WHERE aname LIKE ?;";
            author = author.replace("!", "!!").replace("%", "!%").replace("_", "!_").replace("[", "![");
            PreparedStatement pstmt = con.prepareStatement(psql);
            pstmt.setString(1, "%" + author + "%");
            ResultSet bookList = pstmt.executeQuery();

            if (!bookList.isBeforeFirst()) {
                System.out.println("No book were found!");
                return;
            }

            ArrayList<String> callNums = new ArrayList<String>();
            while (bookList.next()) {
                callNums.add(bookList.getString(1));
            }
            printBook(callNums);
        }
        catch(Exception e) {
            System.out.println(e);
        }
    }

    private static void printBook (ArrayList<String> callNums) {
        try {
            // add all authors to call num
            Hashtable<String, StringBuilder> callNumToAuthor = new Hashtable<String, StringBuilder>();
            StringBuilder callNumList = new StringBuilder();
            for (String callNum : callNums) {
                callNumToAuthor.put(callNum, new StringBuilder());
            }
            int maxAuthorLength = getBookAuthor(callNumToAuthor, callNumList);

            // get book detail for printing
            Statement stmt = con.createStatement();
            String sql = "SELECT book.callnum, book.title, book.rating, book_category.bcname FROM book, book_category WHERE callnum in (" + callNumList.toString() + ") AND book.bcid=book_category.bcid ORDER BY book.callnum ASC;";
            ResultSet bookList = stmt.executeQuery(sql);

            // add all availiable copy to call num
            Hashtable<String, Integer> callNumToAvailable = new Hashtable<String, Integer>();
            stmt = con.createStatement();
            sql = "SELECT callnum, MAX(copynum) FROM book_copy WHERE callnum in (" + callNumList.toString() + ") GROUP BY callnum ORDER BY callnum ASC;";
            ResultSet numOfCopy = stmt.executeQuery(sql);
            while (numOfCopy.next()) {
                String callNum = numOfCopy.getString(1);
                Integer copyNum = numOfCopy.getInt(2);
                callNumToAvailable.put(callNum, copyNum);
            }

            // substract borrowed copy to available copy of call num
            stmt = con.createStatement();
            sql = "SELECT callnum, COUNT(*) FROM borrow WHERE callnum in (" + callNumList.toString() + ") AND return_date IS NULL GROUP BY callnum ORDER BY callnum ASC;";
            ResultSet numOfUnreturn = stmt.executeQuery(sql);
            while (numOfUnreturn.next()) {
                String callNum = numOfUnreturn.getString(1);
                Integer unreturnNum = numOfUnreturn.getInt(2);
                Integer copyNum = callNumToAvailable.get(callNum);
                callNumToAvailable.put(callNum, copyNum - unreturnNum);
            }

            // printing all book
            String format = String.format("|%%-%ds|%%-%ds|%%-%ds|%%-%ds|%%-%ds|%%-%ds|\n", 8, 30, 30, maxAuthorLength, 6, 21);
            System.out.format(format, "Call Num", "Title", "Book Category", "Author", "Rating", "Available No. of Copy");
            while(bookList.next()) {
                String callNum = bookList.getString(1);
                String bookTitle = bookList.getString(2);
                Double r = (Double)bookList.getObject(3);
                String rating;
                if(r == null) rating = "-/-";
                else rating = String.format("%.1f", r);
                String category = bookList.getString(4);
                Integer available = callNumToAvailable.get(callNum);
                String author = callNumToAuthor.get(callNum).toString();
                System.out.format(format, callNum, bookTitle, category, author, rating, available);
            }
            System.out.println("End of Query");
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }

    private static int getBookAuthor (Hashtable<String, StringBuilder> callNumToAuthor, StringBuilder callNumList) throws Exception {
        // concatenate all call num seperated by ", "
        callNumToAuthor.forEach((k, v) -> {
            callNumList.append("'");
            callNumList.append(k);
            callNumList.append("',");
        });
        if(callNumList.length() > 0) callNumList.setLength(callNumList.length() - 1);

        // get all author name
        Statement stmt = con.createStatement();
        String sql = "SELECT * FROM authorship WHERE callnum IN (" + callNumList.toString() + ");";
        ResultSet authors = stmt.executeQuery(sql);

        // map call num to author
        int maxLen = 0;
        while(authors.next()) {
            String callNum = authors.getString(1);
            String author = authors.getString(2);
            StringBuilder authorList = callNumToAuthor.get(callNum);
            if(authorList.length() > 0) authorList.append(", ");
            authorList.append(author);
            maxLen = Math.max(maxLen, authorList.length());
        }

        return maxLen;
    }

    private static void showLoanRecord () {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Enter The User ID: ");
        String libuid = scanner.nextLine();

        try {
            String psql = "SELECT callnum, copynum, checkout, return_date FROM borrow WHERE libuid = ? ORDER BY checkout DESC;";
            PreparedStatement pstmt = con.prepareStatement(psql);
            pstmt.setString(1, libuid);
            ResultSet borrowRecords = pstmt.executeQuery();

            if (!borrowRecords.isBeforeFirst()) {
                System.out.println("No borrow records were found!");
                return;
            }
            
            // map call num to author
            StringBuilder callNumList = new StringBuilder();
            Hashtable<String, StringBuilder> callNumToAuthor = new Hashtable<String, StringBuilder>();
            while(borrowRecords.next()) {
                String callNum = borrowRecords.getString(1);
                if(!callNumToAuthor.containsKey(callNum)) {
                    callNumToAuthor.put(callNum, new StringBuilder());
                }
            }
            int maxAuthorLength = getBookAuthor(callNumToAuthor, callNumList);
            
            // map call num to title
            Hashtable<String, String> callNumToTitle = new Hashtable<String, String>();
            Statement stmt = con.createStatement();
            String sql = "SELECT callnum, title FROM book WHERE callnum IN (" + callNumList.toString() + ");";
            ResultSet bookTitles = stmt.executeQuery(sql);
            while(bookTitles.next()) {
                String callNum = bookTitles.getString(1);
                String bookTitle = bookTitles.getString(2);
                callNumToTitle.put(callNum, bookTitle);
            }

            // print all book
            String format = String.format("|%%-%ds|%%-%ds|%%-%ds|%%-%ds|%%-%ds|%%-%ds|\n", 8, 8, 30, maxAuthorLength, 10, 9);
            System.out.format(format, "Call Num", "Copy Num", "Title", "Author", "Check-out", "Returned?");
            borrowRecords.beforeFirst();
            while(borrowRecords.next()) {
                String callNum = borrowRecords.getString(1);
                String copyNum = Integer.toString(borrowRecords.getInt(2));
                String bookTitle = callNumToTitle.get(callNum);
                String authorList = callNumToAuthor.get(callNum).toString();
                String checkout = borrowRecords.getDate(3).toString();
                String returned = (borrowRecords.getDate(4) != null ? "Yes" : "No");
                System.out.format(format, callNum, copyNum, bookTitle, authorList, checkout, returned);
            }
            System.out.println("End of Query");
        }
        catch (Exception e) {
            System.out.println(e);
        }
    }
}