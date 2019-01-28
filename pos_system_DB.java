package pos;import java.util.*;
import java.util.Date;
import java.text.*;
import java.sql.*;
public class pos {
	
 
	
	
	  static Scanner scan = new Scanner(System.in);
	    public static void main(String args[]) {
	    	Date date = new Date();
	        Connection con = null;
	        
	        try {
	        	Class.forName("com.mysql.jdbc.Driver").newInstance() ;
	           	PreparedStatement pstmt = null;
	           	
	            con = DriverManager.getConnection("jdbc:mysql://peace.handong.edu", "db21200446", "21200446");
	            if (!con.isClosed())
	            System.out.println("Successfully connected to MySQL server.");
	            
	            Statement stmt = (Statement) con.createStatement();
	            PreparedStatement insert = con.prepareStatement("insert into stock(ISBN,title,author,category,copy) values(?,?,?,?,?)");
	            stmt.execute("use 21200446_db");
			            
	            stmt.executeUpdate ("DROP TABLE IF EXISTS stock");
	            stmt.executeUpdate(
	                "create table stock (" + "ISBN INT primary key,"+ "title char(40) , " + "author char(10),"+"category char(10),"+"copy INT)"
	            );
	            
	            stmt.executeUpdate ("DROP TABLE IF EXISTS purchase");
	            stmt.executeUpdate(
	                "create table purchase (" + "ISBN INT ," + "customer char(40) , primary key(ISBN,customer))"
	            );
	            
	            stmt.executeUpdate ("DROP TABLE IF EXISTS report");
	            stmt.executeUpdate(
	                "create table report (" +"ISBN INT primary key , "+"title char(40), " + "sold INT , date Int)"
	            );
	            try{
	            con.setAutoCommit(false);
	            						//trasaction ������� �Ϸ��� query�� Ʈ������ ���.
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(1,'BigBang','jonh','science',4)");
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(2,'jojo','han','history',3)");
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(3,'ant','berber','fiction',2)");
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(4,'bible','god','theology',4)");
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(5,'love','kim','fiction',5)");
	            stmt.executeUpdate("Insert into stock(ISBN,title,author,category,copy) values(6,'iron man','tony','SF',7)");
	            // �ʱ� db�� ��� �ִ� ��.
	            }
	            catch(Exception ex){
	            	con.rollback();
	            	System.out.println("\n**initial data are lost**\n");
	            	
	            }
	            con.commit();
	            con.setAutoCommit(true);
	            System.out.println(" Input number among 1 , 2 , 3 , 4 , 5\n"
	            					+ "1. stock\n"
	            					+ "2. purchase\n"
	            					+ "3. search\n"
	            					+ "4. daily report\n"
	            					+ "5. quit program");
	            String query ="";
	            
	            query = scan.next();
	            
	            while(!query.equals("5"))
	            {
	                if(query.equals(""))
	                    continue;
	                
	                if(query.equals("1")) // stock
	                {
	                	
	                    System.out.println("input following Info sequencially\n"
	                    		+ " <ISBN>\n"
	                    		+ " <title>\n"
	                    		+ " <authorName>\n"
	                    		+ " <category>\n"
	                    		+ " <copy>" );
	                    System.out.print("ISBN :");
	                    String newISBN = scan.next();		//ISBN�� �մ� ������ ��ĥ ��츦 select�̿��ؼ� �����ֱ�. (�߸��� �Է�)
	                    System.out.println("");
	                    
	                    
	                    	try{	
	                    int intISBN=Integer.parseInt(newISBN);	//�̸��� ���� ��ΰ� �մ� ������ ���� ��� �ߺ� �����ֱ�.  (�߸��� �Է�)
	                    //String sql ="select * from stock where ISBN ="+intISBN;
	                    	}
	                    	catch(Exception Q){
	                    		System.out.println("invalid input , try it again.");
	                    		continue;
	                    	}
	                    	int intISBN=Integer.parseInt(newISBN);
	                    System.out.print("title :");
	                    String newTitle = scan.next();
	                    System.out.println("");
	                    System.out.print("author :");
	                    String newAuthor = scan.next();
	                    System.out.println("");
	                    System.out.print("category (IF you wanna skip category, input <null> :");
	                    String newCategory = scan.next();
	                    System.out.println("");
	                    System.out.print("# of copy :");
	                    String newCopy = scan.next();		
	                    System.out.println("");try{
	                    int intCopy=Integer.parseInt(newCopy);
	                    }
	                    catch(Exception Q){
	                    	System.out.println("invalid input , try it again.");
	                    	continue;
	                    }
	                    int intCopy=Integer.parseInt(newCopy);
	                    String sql ="select * from stock where ISBN ="+intISBN;
		                ResultSet rs = stmt.executeQuery(sql);
		                if(!rs.next()){
		                	
		                	
		                	
		                	if(!newCategory.equals("null"))
		                stmt.executeUpdate("insert into stock(ISBN,title,author,category,copy) values('"+intISBN+"','"+newTitle+"','"
	                    +newAuthor+"','"+newCategory +"','"+ intCopy +"');");
		                	else
		                stmt.executeUpdate("insert into stock(ISBN,title,author,category,copy) values('"+intISBN+"','"+newTitle+"','"
		        	    +newAuthor+"','"+"Misc" +"','"+ intCopy +"');");
		                
		                }
		                else
		                {
		                	System.out.println("**there is same ISBN Number**\n");
		                	//�̹� �ִ� ISBN�� ���� ������ �������� �ϸ�, �ٽ� �޴��� ���ư��� �ٽ� ��� ���� �� �Է�.
		                	
		                }
		                System.out.println("stock list (ISBN,TITLE,AUTHOR,CATEGORY,COPIES):");
		                sql = "select * from stock";
		                rs= stmt.executeQuery(sql);
		                while(rs.next()){
		                	System.out.print(rs.getInt(1)+" ");
		                	System.out.print(rs.getString(2)+" ");
		                	System.out.print(rs.getString(3)+" ");
		                	System.out.print(rs.getString(4)+" ");
		                	System.out.print(rs.getInt(5));
		                	System.out.println("");
		                	
		                }
		                
		                
	                }
	                
	                else if(query.equals("2"))	//purchase ISBN�� Ŀ���͸� ���� �޾Ƽ� purchase �ϸ� copy ������ stock���� �ϳ� ������ ������Ʈ, report�� ���̸��� Ŀ���͸� ���� �Է�.
	                {							//��� ���(1,2,3,4 �Ŀ� copy�� 0�� tuple�� ����Ʈ������.
	                	System.out.println("input following Info sequencially\n"
	                    		+ " <ISBN>\n"
	                    		+ " <customer Name>\n"
	                    		);
	                	System.out.print("ISBN :");
	                    String newISBN = scan.next();	//ISBN �˻�.
	                    
	                    try{
	                    	int intISBN=Integer.parseInt(newISBN);
	                    }
	                    catch(Exception Q){
	                    	System.out.println("invalid input , try it again.");
	                    	continue;
	                    }
	                    int intISBN=Integer.parseInt(newISBN);	//int�� ����� �´��� ����ó��
	                    
	                    System.out.println("");
	                    System.out.print("Customer Name :");
	                    String newCustomer = scan.next();
	                    System.out.println("");
	                    try{ 
	                    ResultSet rs;
	                    ResultSet rs2;
	                    String sql = "select * from stock where ISBN = ?";                       
	                    
	                    pstmt = con.prepareStatement(sql);                          // prepareStatement���� �ش� sql�� �̸� �������Ѵ�.
	                    pstmt.setInt(1, intISBN);
	                    rs = pstmt.executeQuery(); 
	                    
	                    while(rs.next()){
	                    	int temp = rs.getInt(5)-1;
	                    	sql = "update stock set copy ="+ temp +" where ISBN="+intISBN; 
	                    	   stmt.executeUpdate(sql);
	                    	 int today=  date.getDate();
	                    	
	                    	sql = "select * from report where title ='"+rs.getString(2)+"'"; 
	   	                    rs2 = stmt.executeQuery(sql);
	   	                    if(rs2.next()){
	   	                    	temp= rs2.getInt(3)+1;
	   	                    	sql = "update report set sold ="+ temp +" where ISBN="+intISBN; 
		                    	   stmt.executeUpdate(sql); 	//���ϸ� ����Ʈ�� �ش� å�� sold ���� + 1
		                    	sql = "update report set date =" + today;
		                    	stmt.executeUpdate(sql);
	   	                    	}
	   	                    else{
		                    	stmt.executeUpdate("insert into report(ISBN,title,sold,date) values('"+rs.getInt(1) +"','" + rs.getString(2)+"',"+1+","+today+")");
	   	                    }
	   	                    
	   	                    
	   	                    
	   	                    //purchase ���̺� insert
	   	                    
	   	                    sql = "insert into purchase(ISBN,customer) values("+rs.getInt(1) +",'"+newCustomer+"')";
	   	                    stmt.executeUpdate(sql);
	   	                    // ���� ����� ���� ISBN�� ������ ���� �ÿ��� �ѹ��� insert��. ����ó�� ���ֱ�.
	   	                    
	   	                }
	                    
	                    sql = "delete from stock where copy=0"; 
	                    stmt.executeUpdate(sql); 
	                    
	                    //list ���� �̾Ƴ���.
	                    /*
	                    sql = "select * from stock"; 
	                    rs = stmt.executeQuery(sql); 
	                    System.out.println("list of stock");
	                    while(rs.next()){ 
	                     System.out.println( rs.getInt(1) +" "+rs.getString(2) +" "+rs.getString(3) +" "+rs.getString(4) +" "+ rs.getInt(5)  ); 
	                    
	                    }
	                    sql = "select * from report"; 
	                    rs = stmt.executeQuery(sql); 
	                    System.out.println("list of report");
	                    while(rs.next()){ 
	                     System.out.println( rs.getInt(1) +" "+rs.getString(2) +" "+rs.getString(3)+" "+rs.getInt(4)  ); 
	                    
	                    }
	                    */
	                    sql = "select * from purchase"; 
	                    rs = stmt.executeQuery(sql); 
	                    System.out.println("list of purchase (ISBN, Customer_Name) : ");
	                    while(rs.next()){ 
	                     System.out.println( rs.getInt(1) +" "+rs.getString(2)); 
	                    
	                    }
	                    //report�� sold �� ���� ���Ʒ��� insert �ϱ�
	                    
	                    
	                   
	                   }
	                   catch(Exception e){
	                	   System.out.print("error");
	                   }
	                   
	                    
	                    
	                   
	                }
	                else if ( query.equals("3")){  // search
	                	
	                	String sql;
	                	ResultSet rs;
	                	System.out.println("input keyword that you want to search");
	                	String search = scan.next();
	                	
	                	System.out.println("search list (ISBN,title,author,category,remaining copies : ");
	                	
	                	try{
	                		int test= Integer.parseInt(search);	//���ĺ����� �������� Ȯ��.
	                	}
	                	catch(Exception W){
	                			//���ĺ��� ����.
	                		
	                		sql = "select * from stock where title REGEXP '"+search+"' OR author REGEXP '" + search +"'"
		                			+ " OR category ='" + search + "'" ;
		                		
	   	                    rs = stmt.executeQuery(sql);
	   	                    
		                	while(rs.next()){
		                		System.out.print(rs.getInt(1)+" ");
		                		System.out.print(rs.getString(2)+" ");
		                		System.out.print(rs.getString(3)+" ");
		                		System.out.print(rs.getString(4)+" ");
		                		System.out.print(rs.getInt(5)+ " \n");
		                	}
	                		continue;
	                		
	                	}
	                	
	                	sql= "select * from stock where ISBN = "+ Integer.parseInt(search) +" OR category ='" + search + "' OR copy =" +Integer.parseInt(search)
	                			+ " OR copy =" +Integer.parseInt(search) ;
	                		
   	                    rs = stmt.executeQuery(sql);
   	                    
	                	while(rs.next()){
	                		System.out.print(rs.getInt(1)+" ");
	                		System.out.print(rs.getString(2)+" ");
	                		System.out.print(rs.getString(3)+" ");
	                		System.out.print(rs.getString(4)+" ");
	                		System.out.print(rs.getInt(5)+ " \n");
	                	}
   	                    
	                	
	                	
	                	
	                
	                	
	                	System.out.println("\n");
	                	
	                	
	                }
	                else if ( query.equals("4"))
	                {
	                	String sql;
	                	ResultSet rs;
	                	sql = "select * from report order by sold DESC";
	                	rs = stmt.executeQuery(sql);
	                	System.out.println("list of daily report : (ISBN, book_title, #_of_sold, sold_date_only day)");
	                	while(rs.next()){
	                		System.out.print(rs.getInt(1)+" ");
	                		System.out.print(rs.getString(2)+" ");
	                		System.out.print(rs.getInt(3)+" ");
	                		System.out.print(rs.getInt(4)+" \n");
	                		
	                	}
	                	
	                	
	                	
	                		}
	                
	                System.out.println(" Input number among 1 , 2 , 3 , 4 , 5\n"
        					+ "1. stock\n"
        					+ "2. purchase\n"
        					+ "3. search\n"
        					+ "4. daily report\n"
        					+ "5. quit program");
	                
 
	                query = "";
	                query = scan.next();
 
 
	            }
	            
	           
	           //---------------
	            stmt.close();
	            insert.close();
	            
	            
	            
	        } 
	        
	        
	      catch(Exception e) {
	               System.err.println("Exception: " + e.getMessage());
	           } 
	      finally {
	               try {
	                   if (con != null)
	                       con.close();
	          } 
	          catch(SQLException e) {}
	      }
	
	    
	    }
 
	
	
}