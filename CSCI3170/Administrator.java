import java.text.SimpleDateFormat;
import java.util.Scanner;
import java.io.File;
import java.util.Date;
import java.sql.*;

class Administrator {
    private static Connection con = null;
    private static String title = "Operations for administrator menu";
    private static String[] optionList = {
        "Create all tables",
        "Delete all tables",
        "Load from datafile",
        "Show number of records in each table",
        "Return to the main menu"
    };

    public static void main (Connection con) {
        Administrator.con = con;

        while (true) {
            int option = CSCI3170Proj.getMenuChoice(title, optionList);
            try {
                switch(option) {
                    case 1:
                        createAllTable();
                        break;
                    case 2:
                        deleteAllTable();
                        break;
                    case 3:
                        loadDataFile();
                        break;
                    case 4:
                        showNumOfRecord();
                        break;
                    case 5:
                        return;
                }
            }
            catch (SQLException e) {
                System.out.println(e);
            }
        }
    }

    private static void createAllTable () throws SQLException {
        System.out.print("Processing...");
        Statement stmt = con.createStatement();
        StringBuilder sql = new StringBuilder();
        
        // dropping all table before creating
        sql.append("DROP TABLE IF EXISTS borrow, authorship, book_copy, book, book_category, libuser, user_category;");
        
        // creating all tables
        sql.append("CREATE TABLE user_category (ucid tinyint NOT NULL, max_book tinyint NOT NULL, loan_period tinyint NOT NULL, PRIMARY KEY (ucid));");
        sql.append("CREATE TABLE libuser (libuid char(10) NOT NULL, uname char(25) NOT NULL, uage tinyint NOT NULL, uaddress char(100) NOT NULL, ucid tinyint NOT NULL, PRIMARY KEY (libuid), FOREIGN KEY (ucid) REFERENCES user_category(ucid));");
        sql.append("CREATE TABLE book_category (bcid tinyint NOT NULL, bcname char(30) NOT NULL, PRIMARY KEY(bcid));");
        sql.append("CREATE TABLE book (callnum char(8) NOT NULL, title char(30) NOT NULL, publish date NOT NULL, rating real DEFAULT NULL, tborrowed tinyint DEFAULT 0, bcid tinyint NOT NULL, PRIMARY KEY (callnum), FOREIGN KEY (bcid) REFERENCES book_category(bcid));");
        sql.append("CREATE TABLE book_copy (callnum char(8) NOT NULL, copynum tinyint NOT NULL, PRIMARY KEY (callnum, copynum), FOREIGN KEY (callnum) REFERENCES book(callnum));");
        sql.append("CREATE TABLE authorship (callnum char(8) NOT NULL, aname char(25) NOT NULL, PRIMARY KEY (callnum, aname), FOREIGN KEY (callnum) REFERENCES book(callnum));");
        sql.append("CREATE TABLE borrow (callnum char(8) NOT NULL, copynum tinyint NOT NULL, libuid char(10) NOT NULL, checkout date NOT NULL, return_date date DEFAULT NULL, PRIMARY KEY (callnum, copynum, libuid, checkout), FOREIGN KEY (libuid) REFERENCES libuser(libuid), FOREIGN KEY (callnum, copynum) REFERENCES book_copy(callnum, copynum));");
        
        stmt.executeUpdate(sql.toString());
        System.out.println("Done. Database is initialized.");
    }

    private static void deleteAllTable () throws SQLException {
        System.out.print("Processing...");
        Statement stmt = con.createStatement();

        // dropping all table if exist
        String sql = "DROP TABLE IF EXISTS borrow, authorship, book_copy, book, book_category, libuser, user_category;";

        stmt.executeUpdate(sql);
        System.out.println("Done. Database is removed.");
    }

    private static void loadDataFile () throws SQLException {
        // getting the file path from user
        Scanner scanner = new Scanner(System.in);
        System.out.print("Type in the Source Data Folder Path: ");
        String path = scanner.nextLine();

        System.out.print("Processing...");
        try {
            // getting the file
            File user_category = new File(path + "/user_category.txt");
            File user = new File(path + "/user.txt");
            File book_category = new File(path + "/book_category.txt");
            File book = new File(path + "/book.txt");
            File check_out = new File(path + "/check_out.txt");
            // report if any of the file is missing
            if(!user_category.exists() || !user.exists() || !book_category.exists() || !book.exists() || !check_out.exists()) {
                System.out.println("\n[Error]: Some files are missing!");
                return;
            }

            // reading the user_category.txt
            Scanner fileReader = new Scanner(user_category);
            String psql = "INSERT INTO user_category VALUES (?, ?, ?);";
            PreparedStatement pstmt = con.prepareStatement(psql);
            while (fileReader.hasNextLine() && fileReader.hasNext()) {
                String input = fileReader.nextLine();
                String[] cols = input.split("\\t");
                pstmt.setInt(1, Integer.parseInt(cols[0]));
                pstmt.setInt(2, Integer.parseInt(cols[1]));
                pstmt.setInt(3, Integer.parseInt(cols[2]));
                pstmt.addBatch();
            }
            pstmt.executeBatch();
            if(fileReader != null) fileReader.close();

            // reading the user.txt
            fileReader = new Scanner(user);
            psql = "INSERT INTO libuser VALUES (?, ?, ?, ?, ?);";
            pstmt = con.prepareStatement(psql);
            while (fileReader.hasNextLine() && fileReader.hasNext()) {
                String input = fileReader.nextLine();
                String[] cols = input.split("\\t");
                pstmt.setString(1, cols[0]);
                pstmt.setString(2, cols[1]);
                pstmt.setInt(3, Integer.parseInt(cols[2]));
                pstmt.setString(4, cols[3]);
                pstmt.setInt(5, Integer.parseInt(cols[4]));
                pstmt.addBatch();
            }
            pstmt.executeBatch();
            if(fileReader != null) fileReader.close();

            // reading the book_category.txt
            fileReader = new Scanner(book_category);
            psql = "INSERT INTO book_category VALUES (?, ?);";
            pstmt = con.prepareStatement(psql);
            while (fileReader.hasNextLine() && fileReader.hasNext()) {
                String input = fileReader.nextLine();
                String[] cols = input.split("\\t");
                pstmt.setInt(1, Integer.parseInt(cols[0]));
                pstmt.setString(2, cols[1]);
                pstmt.addBatch();
            }
            pstmt.executeBatch();
            if(fileReader != null) fileReader.close();

            // reading the book.txt
            fileReader = new Scanner(book);
            psql = "INSERT INTO book VALUES (?, ?, ?, ?, ?, ?);";
            String psql1 = "INSERT INTO book_copy VALUES (?, ?);";
            String psql2 = "INSERT INTO authorship VALUES (?, ?);";
            pstmt = con.prepareStatement(psql);
            PreparedStatement pstmt1 = con.prepareStatement(psql1);
            PreparedStatement pstmt2 = con.prepareStatement(psql2);
            while (fileReader.hasNextLine() && fileReader.hasNext()) {
                String input = fileReader.nextLine();
                String[] cols = input.split("\\t");

                pstmt.setString(1, cols[0]);
                pstmt.setString(2, cols[2]);
                Date publish = new SimpleDateFormat("dd/MM/yyyy").parse(cols[4]);
                pstmt.setDate(3, new java.sql.Date(publish.getTime()));
                Double rating = null;
                try {
                    rating = Double.parseDouble(cols[5]);
                }
                catch (Exception e) {}
                if(rating != null) pstmt.setDouble(4, rating);
                else pstmt.setNull(4, java.sql.Types.REAL);
                pstmt.setInt(5, Integer.parseInt(cols[6]));
                pstmt.setInt(6, Integer.parseInt(cols[7]));
                pstmt.addBatch();
                
                int numOfCopy = Integer.parseInt(cols[1]);
                for(int i = 1; i <= numOfCopy; i++) {
                    pstmt1.setString(1, cols[0]);
                    pstmt1.setInt(2, i);
                    pstmt1.addBatch();
                }

                String[] authors = cols[3].split(",");
                for(String author : authors)
                {
                    pstmt2.setString(1, cols[0]);
                    pstmt2.setString(2, author);
                    pstmt2.addBatch();
                }
            }
            pstmt.executeBatch();
            pstmt1.executeBatch();
            pstmt2.executeBatch();
            if(fileReader != null) fileReader.close();

            // reading the check_out.txt
            fileReader = new Scanner(check_out);
            psql = "INSERT INTO borrow VALUES (?, ?, ?, ?, ?);";
            pstmt = con.prepareStatement(psql);
            while (fileReader.hasNextLine() && fileReader.hasNext()) {
                String input = fileReader.nextLine();
                String[] cols = input.split("\\t");
                pstmt.setString(1, cols[0]);
                pstmt.setInt(2, Integer.parseInt(cols[1]));
                pstmt.setString(3, cols[2]);
                Date checkoutDate = new SimpleDateFormat("dd/MM/yyyy").parse(cols[3]);
                pstmt.setDate(4, new java.sql.Date(checkoutDate.getTime()));
                Date returnDate = null;
                java.sql.Date return_date = null;
                try {
                    returnDate = new SimpleDateFormat("dd/MM/yyyy").parse(cols[4]);
                    return_date = new java.sql.Date(returnDate.getTime());
                }
                catch (Exception e) {}
                pstmt.setDate(5, return_date);
                pstmt.addBatch();
            }
            pstmt.executeBatch();
            if(fileReader != null) fileReader.close();
        }
        catch (SQLException e) {
            System.out.println();
            if(e.getErrorCode() == 1146) {
                System.out.println("[Error]: " + "Some tables are missing. Please initialize the database!");
                return;
            }

            if(e.getErrorCode() == 1062) {
                System.out.println("[Error]: " + "Duplicate data input. Some data already exist!");
                return;
            }
            throw e;
        }
        catch (Exception e) {
            System.out.println("\n[Error]: Illegal file format! Failed to load the data!");
            return;
        }
        System.out.println("Done. Data is inputted to the database.");
    }

    private static void showNumOfRecord () throws SQLException {
        Statement stmt = con.createStatement();
        String sql = "SELECT table_name, SUM(table_rows) FROM information_schema.tables WHERE table_schema = 'db22' AND table_name in ('user_category', 'libuser', 'book_category', 'book', 'book_copy', 'authorship', 'borrow') GROUP BY table_name;";
        ResultSet resultSet = stmt.executeQuery(sql);
        
        // report if no table exist
        if (!resultSet.isBeforeFirst()) {
            System.out.println("No table was found!");
            return;
        }

        // printing all table count
        while(resultSet.next()) {
            System.out.println(resultSet.getString(1) + ": " + resultSet.getInt(2));
        }
    }
}