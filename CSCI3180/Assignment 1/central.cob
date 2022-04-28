      *
      * CSCI3180 Principles of Programming Languages
      *
      * --- Declaration ---
      *
      * I declare that the assignment here submitted is original except for
      * source material explicitly acknowledged. I also acknowledge that I am
      * aware of University policy and regulations on honesty in academic work
      * , and of the disciplinary guidelines and procedures applicable to
      * breaches of such policy and regulations, as contained in the website
      * http://www.cuhk.edu.hk/policy/academichonesty/
      *
      * Assignment 1
      * Name : Lai Man Hin
      * Student ID : 1155136167
      * Email Addr : mhlai0@cse.cuhk.edu.hk
      *
       IDENTIFICATION DIVISION.
       PROGRAM-ID.   CENTRAL-PROG.

       ENVIRONMENT DIVISION.
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
       SELECT INPUT-FILE ASSIGN TO "master.txt"
               ORGANIZATION IS LINE SEQUENTIAL.
       SELECT INPUTONE ASSIGN TO "trans711.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT INPUTTWO ASSIGN TO "trans713.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT OUTPUTONE ASSIGN TO "transac_sorted711.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT OUTPUTTWO ASSIGN TO "transac_sorted713.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT OPTIONAL INPUTMERGED ASSIGN TO "transac_sorted.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT OPTIONAL OUTPUT-UPDATED ASSIGN TO "master_updated.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT OPTIONAL OUTPUT-NEG ASSIGN TO "negReport.txt"
               ORGANIZATION IS LINE SEQUENTIAL
               ACCESS IS SEQUENTIAL.
       SELECT SORTWORK ASSIGN TO SORTWRK.
       DATA DIVISION.
       FILE SECTION.
       FD INPUT-FILE.
       01 ACCOUNT.
          02 NAME PIC A(20).
          02 NUMBE PIC 9(16).
          02 PASSWORD PIC 9(6).
          02 BALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       FD INPUTONE.
       01 MYINPUTONE.
          02 IONUMBE PIC 9(16).
          02 IOACTION PIC A(1).
          02 IOAMOUNT PIC 9(5)V9(2).
          02 IOTIME PIC 9(5).
       FD INPUTTWO.
       01 MYINPUTTWO.
          02 ITNUMBE PIC 9(16).
          02 ITACTION PIC A(1).
          02 ITAMOUNT PIC 9(5)V9(2).
          02 ITTIME PIC 9(5).
       FD OUTPUTONE.
       01 MYOUTPUTONE.
          02 OONUMBE PIC 9(16).
          02 OOACTION PIC A(1).
          02 OOAMOUNT PIC 9(5)V9(2).
          02 OOTIME PIC 9(5).
       FD OUTPUTTWO.
       01 MYOUTPUTTWO.
          02 OTNUMBE PIC 9(16).
          02 OTACTION PIC A(1).
          02 OTAMOUNT PIC 9(5)V9(2).
          02 OTTIME PIC 9(5).
       FD INPUTMERGED.
       01 MYINPUTMERGED.
          02 IMNUMBE PIC 9(16).
          02 IMACTION PIC A(1).
          02 IMAMOUNT PIC 9(5)V9(2).
          02 IMTIME PIC 9(5).
       FD OUTPUT-UPDATED.
       01 OUACCOUNT.
          02 OUNAME PIC A(20).
          02 OUNUMBE PIC 9(16).
          02 OUPASSWORD PIC 9(6).
          02 OUBALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       FD OUTPUT-NEG.
       01 ONACCOUNT.
          02 ONNAMEE PIC X(6).
          02 ONNAME PIC A(20).
          02 ONNUMBEE PIC X(17).
          02 ONNUMBE PIC 9(16).
          02 ONBALANCEE PIC X(10).
          02 ONBALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       SD SORTWORK.
       01 MYSORTWORK.
          02 WNUMBE PIC 9(16).
          02 WACTION PIC A(1).
          02 WAMOUNT PIC 9(5)V9(2).
          02 WTIME PIC 9(5).
       WORKING-STORAGE SECTION.
       01 MYOPERATION.
          02 MNUMBE PIC 9(16).
          02 MACTION PIC A(1).
          02 MAMOUNT PIC 9(5)V9(2).
          02 MTIME PIC 9(5).
       01 CURMASTER.
          02 CURNAME PIC A(20).
          02 CURNUMBE PIC 9(16).
          02 CURPASSWORD PIC 9(6).
          02 CURBALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       PROCEDURE DIVISION.
       MYSORT-PARAGRAPH.
              SORT SORTWORK ON ASCENDING KEY OONUMBE
              USING INPUTONE GIVING OUTPUTONE.
              SORT SORTWORK ON ASCENDING KEY OTNUMBE
              USING INPUTTWO GIVING OUTPUTTWO.
              GO TO MYMERGE711-PARAGRAPH.
       MYMERGE711-PARAGRAPH.
              OPEN INPUT OUTPUTONE.
              GO TO COPY711-PARAGRAPH.
       COPY711-PARAGRAPH.
              READ OUTPUTONE INTO MYOPERATION
                 AT END GO TO MYMERGE713-PARAGRAPH
                 NOT AT END GO TO COPY711BYLINE-PARAGRAPH
              END-READ.
       COPY711BYLINE-PARAGRAPH.
              OPEN EXTEND INPUTMERGED.
                MOVE MNUMBE TO IMNUMBE.
                MOVE MACTION TO IMACTION.
                MOVE MAMOUNT TO IMAMOUNT.
                MOVE MTIME TO IMTIME.
                WRITE MYINPUTMERGED
                END-WRITE.
              CLOSE INPUTMERGED.
              GO TO COPY711-PARAGRAPH.
       MYMERGE713-PARAGRAPH.
              CLOSE OUTPUTONE.
              OPEN INPUT OUTPUTTWO.
              GO TO COPY713-PARAGRAPH.
       COPY713-PARAGRAPH.
              READ OUTPUTTWO INTO MYOPERATION
                 AT END GO TO FINALSORT-PARAGRAPH
                 NOT AT END GO TO COPY713BYLINE-PARAGRAPH
              END-READ.
       COPY713BYLINE-PARAGRAPH.
              OPEN EXTEND INPUTMERGED.
                MOVE MNUMBE TO IMNUMBE.
                MOVE MACTION TO IMACTION.
                MOVE MAMOUNT TO IMAMOUNT.
                MOVE MTIME TO IMTIME.
                WRITE MYINPUTMERGED
                END-WRITE.
              CLOSE INPUTMERGED.
              GO TO COPY713-PARAGRAPH.
       FINALSORT-PARAGRAPH.
              CLOSE OUTPUTTWO.
              SORT SORTWORK ON ASCENDING KEY IMNUMBE
                 ASCENDING KEY IMTIME
              USING INPUTMERGED GIVING INPUTMERGED.
              GO TO PREPAREMASTER-PARAGRAPH.
       PREPAREMASTER-PARAGRAPH.
              OPEN INPUT INPUT-FILE.
              OPEN INPUT INPUTMERGED.
              GO TO FIRSTMASTER-PARAGRAPH.
       FIRSTMASTER-PARAGRAPH.
              READ INPUT-FILE INTO CURMASTER
                 AT END GO TO TERMINATE-PARAGRAPH
                 NOT AT END GO TO FORSORTED-PARAGRAPH
              END-READ.
       FORSORTED-PARAGRAPH.
              READ INPUTMERGED
                 AT END
                    CLOSE INPUTMERGED
                    GO TO REMAININGMASTER-PARAGRAPH
                 NOT AT END
                    IF IMNUMBE NOT = NUMBE
                       GO TO WRITEMASTER-PARAGRAPH
                    END-IF
              END-READ.
              GO TO CALCULATION-PARAGRAPH.
       WRITEMASTER-PARAGRAPH.
              IF CURBALANCE < 0 THEN
                 OPEN EXTEND OUTPUT-NEG
                   MOVE "Name: " TO ONNAMEE
                   MOVE CURNAME TO ONNAME
                   MOVE " Account Number: " TO ONNUMBEE
                   MOVE CURNUMBE TO ONNUMBE
                   MOVE " Balance: " TO ONBALANCEE
                   MOVE CURBALANCE TO ONBALANCE
                   WRITE ONACCOUNT
                 END-WRITE
                 CLOSE OUTPUT-NEG
              END-IF.
              OPEN EXTEND OUTPUT-UPDATED.
                   MOVE CURNAME TO OUNAME.
                   MOVE CURNUMBE TO OUNUMBE.
                   MOVE CURPASSWORD TO OUPASSWORD.
                   MOVE CURBALANCE TO OUBALANCE.
                   WRITE OUACCOUNT
              END-WRITE.
              CLOSE OUTPUT-UPDATED.
              READ INPUT-FILE INTO CURMASTER.
              IF IMNUMBE NOT = NUMBE
                 GO TO WRITEMASTER-PARAGRAPH
              END-IF.
       CALCULATION-PARAGRAPH.
              IF IMACTION = 'D' THEN
                 COMPUTE CURBALANCE = CURBALANCE + IMAMOUNT
              END-IF.
              IF IMACTION = 'W' THEN
                 COMPUTE CURBALANCE = CURBALANCE - IMAMOUNT
              END-IF.
              GO TO FORSORTED-PARAGRAPH.
       REMAININGMASTER-PARAGRAPH.
              IF CURBALANCE < 0 THEN
                 OPEN EXTEND OUTPUT-NEG
                   MOVE "Name: " TO ONNAMEE
                   MOVE CURNAME TO ONNAME
                   MOVE " Account Number: " TO ONNUMBEE
                   MOVE CURNUMBE TO ONNUMBE
                   MOVE " Balance: " TO ONBALANCEE
                   MOVE CURBALANCE TO ONBALANCE
                   WRITE ONACCOUNT
                 END-WRITE
                 CLOSE OUTPUT-NEG
              END-IF.
              OPEN EXTEND OUTPUT-UPDATED.
                       MOVE CURNAME TO OUNAME.
                       MOVE CURNUMBE TO OUNUMBE.
                       MOVE CURPASSWORD TO OUPASSWORD.
                       MOVE CURBALANCE TO OUBALANCE.
              WRITE OUACCOUNT
              END-WRITE.
              CLOSE OUTPUT-UPDATED.
              READ INPUT-FILE
                 AT END
                    CLOSE INPUT-FILE
                    GO TO TERMINATE-PARAGRAPH
                 NOT AT END
                    OPEN EXTEND OUTPUT-UPDATED
                       MOVE NAME TO OUNAME
                       MOVE NUMBE TO OUNUMBE
                       MOVE PASSWORD TO OUPASSWORD
                       MOVE BALANCE TO OUBALANCE
                    WRITE OUACCOUNT
                    END-WRITE
                    CLOSE OUTPUT-UPDATED
                    GO TO REMAININGMASTER-PARAGRAPH
               END-READ.

       TERMINATE-PARAGRAPH.
              STOP RUN.
