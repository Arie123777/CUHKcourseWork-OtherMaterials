       IDENTIFICATION DIVISION.
       PROGRAM-ID.   ATMS-PROG.
       AUTHOR.       LAI MAN HIN.

       ENVIRONMENT DIVISION.
       *> hi
       INPUT-OUTPUT SECTION.
       FILE-CONTROL.
       SELECT INPUT-FILE ASSIGN TO "master.txt"
                ORGANIZATION IS LINE SEQUENTIAL.
       SELECT OPTIONAL OUTPUTONE ASSIGN TO "trans711.txt"
                ORGANIZATION IS LINE SEQUENTIAL
                ACCESS IS SEQUENTIAL.
       SELECT OPTIONAL OUTPUTTWO ASSIGN TO "trans713.txt"
                ORGANIZATION IS LINE SEQUENTIAL
                ACCESS IS SEQUENTIAL.
       DATA DIVISION.
       FILE SECTION.
       FD INPUT-FILE.
       01 ACCOUNT.
           02 NAME PIC A(20).
           02 NUMBE PIC 9(16).
           02 PASSWORD PIC 9(6).
           02 BALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       FD OUTPUTONE.
       01 MYOPERATIONONE.
           02 ONUMBE PIC 9(16).
           02 OACTION PIC A(1).
           02 OAMOUNT PIC 9(5)V9(2).
           02 OTIME PIC 9(5).
       FD OUTPUTTWO.
       01 MYOPERATIONTWO.
           02 TNUMBE PIC 9(16).
           02 TACTION PIC A(1).
           02 TAMOUNT PIC 9(5)V9(2).
           02 TTIME PIC 9(5).
       WORKING-STORAGE SECTION.
       01 AACCOUNT.
           02 ANAME PIC A(20).
           02 ANUMBE PIC 9(16).
           02 APASSWORD PIC 9(6).
           02 ABALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       01 BACCOUNT.
           02 BNAME PIC A(20).
           02 BNUMBE PIC 9(16).
           02 BPASSWORD PIC 9(6).
           02 BBALANCE PIC S9(13)V9(2) SIGN LEADING SEPARATE.
       01 WS-EOF PIC A(1).
       01 MYATM PIC A(20).
       01 MYNUMBE PIC 9(16).
       01 MYPASSWORD PIC 9(20).
       01 MYSERVICE PIC A(20).
       01 MYAMOUNT PIC 9(13)V9(2).
       01 MYTARGET PIC 9(16).
       01 YN PIC A(20).
       01 MYTIME PIC 9(5) VALUE 0.
       PROCEDURE DIVISION.
       START-PARAGRAPH.
              DISPLAY "##############################################".
              DISPLAY "##         Gringotts Wizarding Bank         ##".
              DISPLAY "##                 Welcome                  ##".
              DISPLAY "##############################################".
              DISPLAY "=> PLEASE CHOOSE THE ATM".
              DISPLAY "=> PRESS 1 FOR ATM 711".
              DISPLAY "=> PRESS 2 FOR ATM 713".
              ACCEPT MYATM.
              IF MYATM NOT = "1" AND MYATM NOT = "2" THEN
                 DISPLAY "=> INVALID INPUT!"
                 GO TO START-PARAGRAPH
              END-IF.
              GO TO USRPWD-PARAGRAPH.
              GO TO TERMINATE-PARAGRAPH.
       USRPWD-PARAGRAPH.
              DISPLAY "=> ACCOUNT".
              ACCEPT MYNUMBE.
              DISPLAY "=> PASSWORD".
              ACCEPT MYPASSWORD.
              GO TO FILEVALI-PARAGRAPH.
       FILEVALI-PARAGRAPH.
              OPEN INPUT INPUT-FILE.
              GO TO FILELINE-PARAGRAPH.
       FILELINE-PARAGRAPH.
              READ INPUT-FILE INTO AACCOUNT
                 AT END GO TO ACCNOTFOUND-PARAGRAPH
                 NOT AT END GO TO LINEVALI-PARAGRAPH
              END-READ.
       ACCNOTFOUND-PARAGRAPH.
              DISPLAY "=> INCORRECT ACCOUNT/PASSWORD".
              CLOSE INPUT-FILE.
              GO TO USRPWD-PARAGRAPH.
       LINEVALI-PARAGRAPH.
              IF ANUMBE = MYNUMBE THEN
                 IF APASSWORD = MYPASSWORD THEN
                    CLOSE INPUT-FILE
                    IF ABALANCE <= 0 THEN
	                DISPLAY "NEGATIVE REMAINS TRANSECTION ABORT"
	                GO TO USRPWD-PARAGRAPH
	             END-IF
                    GO TO SERVICE-PARAGRAPH
                 END-IF
              END-IF.
              GO TO FILELINE-PARAGRAPH.
       SERVICE-PARAGRAPH.
	       DISPLAY "=> PLEASE CHOOSE YOUR SERVICE".
	       DISPLAY "=> PRESS D FOR DEPOSIT".
	       DISPLAY "=> PRESS W FOR WITHDRAWAL".
	       DISPLAY "=> PRESS T FOR TRANSFER".
	       ACCEPT MYSERVICE.
	       IF MYSERVICE = "D" THEN
	          GO TO DEPOSIT-PARAGRAPH
	       END-IF.
	       IF MYSERVICE = "W" THEN
	          GO TO WITHDRAWAL-PARAGRAPH
               END-IF.
	       IF MYSERVICE = "T" THEN
	          GO TO TRANSFER-PARAGRAPH
	       END-IF.
	       DISPLAY "=> INVALID INPUT".
               GO TO SERVICE-PARAGRAPH.
       DEPOSIT-PARAGRAPH.
               DISPLAY "=> AMOUNT".
               ACCEPT MYAMOUNT.
               IF MYAMOUNT > 0 AND MYAMOUNT < 100000 THEN
                  IF MYATM = 1 THEN
                      GO TO TRANS711WRITE-PARAGRAPH
                  END-IF
                  IF MYATM = 2 THEN
                      GO TO TRANS713WRITE-PARAGRAPH
                  END-IF
               END-IF.
               DISPLAY "=> INVALID INPUT".
               GO TO DEPOSIT-PARAGRAPH.
       WITHDRAWAL-PARAGRAPH.
               DISPLAY "=> AMOUNT".
               ACCEPT MYAMOUNT.
               IF MYAMOUNT > 0 AND MYAMOUNT <= ABALANCE AND MYAMOUNT < 100000 THEN
                  IF MYATM = 1 THEN
                      GO TO TRANS711WRITE-PARAGRAPH
                  END-IF
                  IF MYATM = 2 THEN
                      GO TO TRANS713WRITE-PARAGRAPH
                  END-IF
               END-IF.
               IF MYAMOUNT > ABALANCE THEN
                  DISPLAY "=> INSUFFICIENT BALANCE"
                  GO TO WITHDRAWAL-PARAGRAPH
               END-IF.
               DISPLAY "=> INVALID INPUT".
               GO TO WITHDRAWAL-PARAGRAPH.
       TRANSFER-PARAGRAPH.
               DISPLAY "=> TARGET ACCOUNT".
               ACCEPT MYTARGET.
               IF MYTARGET = ANUMBE THEN
                  DISPLAY "=> YOU CANNOT TRANSFER TO YOURSELF"
                  GO TO TRANSFER-PARAGRAPH
               END-IF.
               GO TO BFILEVALI-PARAGRAPH.
       BFILEVALI-PARAGRAPH.
              OPEN INPUT INPUT-FILE.
              GO TO BFILELINE-PARAGRAPH.
       BFILELINE-PARAGRAPH.
              READ INPUT-FILE INTO BACCOUNT
                 AT END GO TO BACCNOTFOUND-PARAGRAPH
                 NOT AT END GO TO BLINEVALI-PARAGRAPH
              END-READ.
       BACCNOTFOUND-PARAGRAPH.
              DISPLAY "=> TARGET ACCOUNT DOES NOT EXIST".
              CLOSE INPUT-FILE.
              GO TO TRANSFER-PARAGRAPH.
       BLINEVALI-PARAGRAPH.
              IF BNUMBE = MYTARGET THEN
                 CLOSE INPUT-FILE
                 GO TO TRANSFERAMOUNT-PARAGRAPH
              END-IF.
              GO TO BFILELINE-PARAGRAPH.
       TRANSFERAMOUNT-PARAGRAPH.
              DISPLAY "=> AMOUNT".
              ACCEPT MYAMOUNT.
              IF MYAMOUNT > 0 AND MYAMOUNT <= ABALANCE THEN
                  IF MYATM = 1 THEN
                      GO TO TRANS711WRITE-PARAGRAPH
                  END-IF
                  IF MYATM = 2 THEN
                      GO TO TRANS713WRITE-PARAGRAPH
                  END-IF
              END-IF.
              IF MYAMOUNT > ABALANCE THEN
                  DISPLAY "=> INSUFFICIENT BALANCE"
                  GO TO WITHDRAWAL-PARAGRAPH
              END-IF.
              DISPLAY "=> INVALID INPUT".
              GO TO TRANSFERAMOUNT-PARAGRAPH.
       TRANS711WRITE-PARAGRAPH.
              IF MYSERVICE = "T" THEN
                 MOVE "W" TO MYSERVICE
                 OPEN EXTEND OUTPUTONE
                    MOVE MYNUMBE TO ONUMBE
                    MOVE MYSERVICE TO OACTION
                    MOVE MYAMOUNT TO OAMOUNT
                    MOVE MYTIME TO OTIME
                    WRITE MYOPERATIONONE
                 END-WRITE
                 COMPUTE MYNUMBE = MYTARGET
                 COMPUTE MYTIME = MYTIME + 1
                 MOVE "D" TO MYSERVICE
                 CLOSE OUTPUTONE
              END-IF.
              OPEN EXTEND OUTPUTONE.
                 MOVE MYNUMBE TO ONUMBE.
                 MOVE MYSERVICE TO OACTION.
                 MOVE MYAMOUNT TO OAMOUNT.
                 MOVE MYTIME TO OTIME.
                 WRITE MYOPERATIONONE
                 END-WRITE.
              CLOSE OUTPUTONE.
              COMPUTE MYTIME = MYTIME + 1.
              GO TO ASKCONTINUE-PARAGRAPH.
       TRANS713WRITE-PARAGRAPH.
              IF MYSERVICE = "T" THEN
                 MOVE "W" TO MYSERVICE
                 OPEN EXTEND OUTPUTTWO
                    MOVE MYNUMBE TO TNUMBE
                    MOVE MYSERVICE TO TACTION
                    MOVE MYAMOUNT TO TAMOUNT
                    MOVE 0 TO TTIME
                    WRITE MYOPERATIONTWO
                 END-WRITE
                 COMPUTE MYNUMBE = MYTARGET
                 COMPUTE MYTIME = MYTIME + 1
                 MOVE "D" TO MYSERVICE
                 CLOSE OUTPUTTWO
              END-IF.
              OPEN EXTEND OUTPUTTWO.
                 MOVE MYNUMBE TO TNUMBE.
                 MOVE MYSERVICE TO TACTION.
                 MOVE MYAMOUNT TO TAMOUNT.
                 MOVE MYTIME TO TTIME.
                 WRITE MYOPERATIONTWO
                 END-WRITE.
              CLOSE OUTPUTTWO.
              COMPUTE MYTIME = MYTIME + 1.
              GO TO ASKCONTINUE-PARAGRAPH.
       ASKCONTINUE-PARAGRAPH.
               DISPLAY "=> CONTINUE?".
               DISPLAY "=>  N FOR NO".
               DISPLAY "=>  Y FOR YES".
               ACCEPT YN.
               IF YN = "Y" THEN
                  GO TO START-PARAGRAPH
               END-IF.
               IF YN = "N" THEN
                  GO TO TERMINATE-PARAGRAPH
               END-IF.
               DISPLAY "=> INVALID INPUT".
               GO TO ASKCONTINUE-PARAGRAPH.
       TERMINATE-PARAGRAPH.
              STOP RUN.
