package ceng.ceng351.cengfactorydb;
import ceng.ceng351.cengfactorydb.*;
import java.sql.*;
import java.sql.Date;
import java.util.*;
import com.mysql.cj.xdevapi.Statement;

import ceng.ceng351.cengfactorydb.QueryResult.MostValueableProduct;

public class CENGFACTORYDB implements ICENGFACTORYDB{
    /**
     * Place your initialization code inside if required.
     *
     * <p>
     * This function will be called before all other operations. If your implementation
     * need initialization , necessary operations should be done inside this function. For
     * example, you can set your connection to the database server inside this function.
     */
    private static String user = "e2521169"; // TODO: Your userName
    private static String password = "e&BR*qrzsKKD"; //  TODO: Your password
    private static String host = "144.122.71.128"; // host name
    private static String database = "db2521169"; // TODO: Your database name
    private static int port = 8080; // port
    private static Connection connection = null;

    public void initialize() 
    {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;

        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, user, password);
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * Should create the necessary tables when called.
     *
     * @return the number of tables that are created successfully.
     */
    public int createTables() 
    {
    	int sayi=0;
    	String t1 = "create table if not exists Factory(factoryId int, factoryName Text, factoryType Text, country Text, primary key(factoryId))";
    	String t2 = "create table if not exists Employee(employeeId int, employeeName Text, department Text, salary int, primary key(employeeId))";
    	String t3 = "create table if not exists Works_In(factoryId int, employeeId int, startDate Date, primary key(factoryId, employeeId), foreign key(employeeId) references Employee on delete cascade on update cascade)";
    	String t4 = "create table if not exists Product(productId int, productName Text, productType Text, primary key(productId))";
    	String t5 = "create table if not exists Produce(factoryId int, productId int, amount int, productionCost int, primary key(factoryId, productId), foreign key(factoryId) references Factory on delete cascade on update cascade, foreign key(productId) references Product on delete cascade on update cascade )";
    	String t6 = "create table if not exists Shipment(factoryId int, productId int, amount int, pricePerUnit int, primary key(factoryId, productId), foreign key(factoryId) references Factory on delete cascade on update cascade, foreign key(productId) references Product on delete cascade on update cascade )";
        try {
            java.sql.Statement st = this.connection.createStatement();

            st.executeUpdate(t1); sayi++;
            st.executeUpdate(t2); sayi++;
            st.executeUpdate(t3); sayi++;
            st.executeUpdate(t4); sayi++;
            st.executeUpdate(t5); sayi++;
            st.executeUpdate(t6); sayi++;

            st.close();
        }

        catch (SQLException e) {
            e.printStackTrace();
        }
        return sayi;
    }

    /**
     * Should drop the tables if exists when called.
     *
     * @return the number of tables are dropped successfully.
     */
    public int dropTables() 
    {
    	int sayi=0;
    	String t1 = "drop table if  exists Factory";
    	String t2 = "drop table if  exists Employee";
    	String t3 = "drop table if  exists Works_In";
    	String t4 = "drop table if  exists Product";
    	String t5 = "drop table if  exists Produce";
    	String t6 = "drop table if  exists Shipment";
        try 
        {
            java.sql.Statement st = this.connection.createStatement();

            st.executeUpdate(t6); sayi++;
            st.executeUpdate(t5); sayi++;
            st.executeUpdate(t4); sayi++;
            st.executeUpdate(t3); sayi++;
            st.executeUpdate(t2); sayi++;
            st.executeUpdate(t1); sayi++;

            st.close();
        }

        catch (SQLException e) 
        {
            e.printStackTrace();
        }
        return sayi;
    }

    /**
     * Should insert an array of Factory into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertFactory(Factory[] factories) 
    {
    	int sayi=0;
        for (int i = 0; i < factories.length; i++)
        {
            try 
            {
                Factory a = factories[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Factory values(?,?,?,?)");
                st.setInt(1,a.getFactoryId());
                st.setString(2,a.getFactoryName());
                st.setString(3,a.getFactoryType());
                st.setString(4,a.getCountry());

                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }

    /**
     * Should insert an array of Employee into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertEmployee(Employee[] employees) 
    {
    	int sayi=0;
        for (int i = 0; i < employees.length; i++)
        {
            try 
            {
                Employee a = employees[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Employee values(?,?,?,?)");
                st.setInt(1,a.getEmployeeId());
                st.setString(2,a.getEmployeeName());
                st.setString(3,a.getDepartment());
                st.setInt(4,a.getSalary());

                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }

    /**
     * Should insert an array of WorksIn into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertWorksIn(WorksIn[] worksIns) 
    {
    	int sayi=0;
        for (int i = 0; i < worksIns.length; i++)
        {
            try 
            {
                WorksIn a = worksIns[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Works_In values(?,?,?)");
                st.setInt(1,a.getFactoryId());
                st.setInt(2,a.getEmployeeId());
                st.setDate(3, Date.valueOf(a.getStartDate()));
                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }

    /**
     * Should insert an array of Product into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduct(Product[] products) 
    {
    	int sayi=0;
        for (int i = 0; i < products.length; i++)
        {
            try 
            {
                Product a = products[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Product values(?,?,?)");
                st.setInt(1,a.getProductId());
                st.setString(2,a.getProductName());
                st.setString(3,a.getProductType());

                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }


    /**
     * Should insert an array of Produce into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertProduce(Produce[] produces) 
    {
    	int sayi=0;
        for (int i = 0; i < produces.length; i++)
        {
            try 
            {
                Produce a = produces[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Produce values(?,?,?,?)");
                st.setInt(1,a.getFactoryId());
                st.setInt(2,a.getProductId());
                st.setInt(3, a.getAmount());
                st.setInt(4, a.getProductionCost());
                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }


    /**
     * Should insert an array of Shipment into the database.
     *
     * @return Number of rows inserted successfully.
     */
    public int insertShipment(Shipment[] shipments) 
    {
    	int sayi=0;
        for (int i = 0; i < shipments.length; i++)
        {
            try 
            {
                Shipment a = shipments[i];

                PreparedStatement st=this.connection.prepareStatement("insert into Shipment values(?,?,?,?)");
                st.setInt(1,a.getFactoryId());
                st.setInt(2,a.getProductId());
                st.setInt(3, a.getAmount());
                st.setInt(4, a.getPricePerUnit());
                st.executeUpdate();

                //Close
                st.close();
                sayi++;

            }
            catch (SQLException e) 
            {
                e.printStackTrace();
            }
        }

        return sayi;
    }

    /**
     * Should return all factories that are located in a particular country.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesForGivenCountry(String country) 
    {
        List<Factory> factories = new ArrayList<>();

        try 
        {
            PreparedStatement st = this.connection.prepareStatement("SELECT F.factoryId, F.factoryName, F.factoryType, F.country FROM Factory F WHERE F.country = ? ORDER BY F.factoryId ASC");
            st.setString(1, country);
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                factories.add(new Factory(rs.getInt("factoryId"), rs.getString("factoryName"), rs.getString("factoryType"), rs.getString("country")));
            }

            //Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return factories.toArray(new Factory[0]);    
    }



    /**
     * Should return all factories without any working employees.
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesWithoutAnyEmployee() 
    {
        List<Factory> factories = new ArrayList<>();

        try 
        {
            PreparedStatement st = this.connection.prepareStatement("SELECT f.factoryId, f.factoryName, f.factoryType, f.country FROM Factory f WHERE NOT EXISTS (SELECT * FROM Works_In w WHERE f.factoryId = w.factoryId) ORDER BY factoryId ASC");
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                factories.add(new Factory(rs.getInt("factoryId"), rs.getString("factoryName"), rs.getString("factoryType"), rs.getString("country")));
            }

            //Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return factories.toArray(new Factory[0]);    
    }

    /**
     * Should return all factories that produce all products for a particular productType
     *
     * @return Factory[]
     */
    public Factory[] getFactoriesProducingAllForGivenType(String productType) 
    {
        List<Factory> factories = new ArrayList<>();

        try {
            String query = "SELECT f.factoryId, f.factoryName, f.factoryType, f.country " +
		                    "FROM Factory f " +
		                    "WHERE NOT EXISTS (" +
		                    "SELECT 1 FROM Product pr " +
		                    "WHERE pr.productType = ? AND NOT EXISTS (" +
		                    "SELECT 1 FROM Produce p " +
		                    "WHERE p.factoryId = f.factoryId AND p.productId = pr.productId)) " +
		                    "ORDER BY f.factoryId ASC";
            PreparedStatement st = this.connection.prepareStatement(query);
            st.setString(1, productType);
            ResultSet rs = st.executeQuery();

            while(rs.next()) {
                factories.add(new Factory(rs.getInt("factoryId"), rs.getString("factoryName"), rs.getString("factoryType"), rs.getString("country")));
            }

            //Close
            rs.close();
            st.close();    
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return factories.toArray(new Factory[0]);
    }

    /**
     * Should return the products that are produced in a particular factory but
     * don’t have any shipment from that factory.
     *
     * @return Product[]
     */
    public Product[] getProductsProducedNotShipped() 
    {
        List<Product> products = new ArrayList<>();

        try {
            String query = "SELECT DISTINCT pr.productId, pr.productName, pr.productType FROM Produce pe, Product pr WHERE pr.productId=pe.productId AND NOT EXISTS ( SELECT * FROM Shipment s WHERE s.productId=pe.productId AND s.factoryId=pe.factoryId) ORDER BY pr.productId ASC";
            PreparedStatement st = this.connection.prepareStatement(query);
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                products.add(new Product(rs.getInt("productId"), rs.getString("productName"), rs.getString("productType")));
            }

            //Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return products.toArray(new Product[0]);    }


    /**
     * For a given factoryId and department, should return the average salary of
     *     the employees working in that factory and that specific department.
     *
     * @return double
     */
    public double getAverageSalaryForFactoryDepartment(int factoryId, String department) 
    {
        double averageSalary = 0.0;

        try 
        {
            String query = "SELECT AVG(e.salary) AS asd FROM Employee e, Works_In w WHERE w.employeeId=e.employeeId AND w.factoryId = ? AND e.department=? ";
            PreparedStatement st = this.connection.prepareStatement(query);
            st.setInt(1, factoryId);
            st.setString(2, department);
            ResultSet rs = st.executeQuery();

            if(rs.next()) 
            {
                averageSalary = rs.getDouble("asd");
            }

            //Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return averageSalary;    
    }


    /**
     * Should return the most profitable products for each factory
     *
     * @return QueryResult.MostValueableProduct[]
     */
    public QueryResult.MostValueableProduct[] getMostValueableProducts() 
    {
        List<MostValueableProduct> profitableProducts = new ArrayList<>();
        try 
        {
            String query = "SELECT f.factoryId, pro.productId, pro.productName, pro.productType, (s3.amount*s3.pricePerUnit - p3.amount*p3.productionCost) AS profit FROM Produce p3, ((SELECT pi3.factoryId, pi3.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi3 WHERE NOT EXISTS ( SELECT 1 FROM Shipment si3 WHERE si3.factoryId = pi3.factoryId AND si3.productId = pi3.productId))  UNION SELECT * FROM Shipment sii3) AS s3, Product pro, Factory f WHERE	(SELECT pr.productId FROM Produce p2, ((SELECT pi2.factoryId, pi2.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi2 WHERE NOT EXISTS ( SELECT 1 FROM Shipment si2 WHERE si2.factoryId = pi2.factoryId AND si2.productId = pi2.productId))  UNION SELECT * FROM Shipment sii2) AS s2, Product pr WHERE s2.amount*s2.pricePerUnit - p2.amount*p2.productionCost=(SELECT MAX(s.amount*s.pricePerUnit - p.amount*p.productionCost) FROM Produce p, ((SELECT pi.factoryId, pi.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi WHERE NOT EXISTS ( SELECT 1 FROM Shipment si WHERE si.factoryId = pi.factoryId AND si.productId = pi.productId))  UNION SELECT * FROM Shipment sii) AS s WHERE p.factoryId=f.factoryId AND s.factoryId=f.factoryId AND p.productId=s.productId) AND p2.factoryId=f.factoryId AND s2.factoryId=f.factoryId AND p2.productId=s2.productId AND p2.productId=pr.productId) = pro.productId AND p3.factoryId=f.factoryId AND s3.factoryId=f.factoryId AND p3.productId=s3.productId AND p3.productId=pro.productId order by profit DESC, factoryId ASC";

            PreparedStatement st = this.connection.prepareStatement(query);
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                profitableProducts.add(new MostValueableProduct(
                    rs.getInt("factoryId"),
                    rs.getInt("productId"),
                    rs.getString("productName"),
                    rs.getString("productType"),
                    rs.getDouble("profit")
                ));
            }

            // Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return profitableProducts.toArray(new MostValueableProduct[0]);
    }
    /**
     * For each product, return the factories that gather the highest profit
     * for that product
     *
     * @return QueryResult.MostValueableProduct[]
     */
    public QueryResult.MostValueableProduct[] getMostValueableProductsOnFactory() 
    
    {
        List<MostValueableProduct> profitableProducts = new ArrayList<>();
        try 
        {
            String query = "SELECT DISTINCT f3.factoryId, pro.productId, pro.productName, pro.productType, (s3.amount*s3.pricePerUnit - p3.amount*p3.productionCost) AS profit FROM Produce pd, Produce p3, ((SELECT pi3.factoryId, pi3.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi3 WHERE NOT EXISTS ( SELECT 1 FROM Shipment si3 WHERE si3.factoryId = pi3.factoryId AND si3.productId = pi3.productId))  UNION SELECT * FROM Shipment sii3) AS s3, Product pro, Factory f3 WHERE	(SELECT DISTINCT f2.factoryId FROM Produce p2, Factory f2, ((SELECT pi2.factoryId, pi2.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi2 WHERE NOT EXISTS ( SELECT 1 FROM Shipment si2 WHERE si2.factoryId = pi2.factoryId AND si2.productId = pi2.productId))  UNION SELECT * FROM Shipment sii2) AS s2, Product pr WHERE s2.amount*s2.pricePerUnit - p2.amount*p2.productionCost=(SELECT MAX(s.amount*s.pricePerUnit - p.amount*p.productionCost) FROM ((SELECT pi.factoryId, pi.productId, 0 as amount, 0 as pricePerUnit FROM Produce pi WHERE NOT EXISTS ( SELECT 1 FROM Shipment si WHERE si.factoryId = pi.factoryId AND si.productId = pi.productId))  UNION SELECT * FROM Shipment sii) AS s, Factory f, Produce p WHERE p.factoryId=f.factoryId AND s.factoryId=f.factoryId AND p.productId=s.productId AND p.productId=pd.productId) AND p2.factoryId=f2.factoryId AND s2.factoryId=f2.factoryId AND p2.productId=s2.productId AND p2.productId=pd.productId) = f3.factoryId AND p3.factoryId=f3.factoryId AND s3.factoryId=f3.factoryId AND p3.productId=s3.productId AND p3.productId=pro.productId AND p3.productId=pd.productId order by profit DESC, f3.factoryId ASC";

            PreparedStatement st = this.connection.prepareStatement(query);
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                profitableProducts.add(new MostValueableProduct(
                    rs.getInt("factoryId"),
                    rs.getInt("productId"),
                    rs.getString("productName"),
                    rs.getString("productType"),
                    rs.getDouble("profit")
                ));
            }

            // Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return profitableProducts.toArray(new MostValueableProduct[0]);
    }


    /**
     * For each department, should return all employees that are paid under the
     *     average salary for that department. You consider the employees
     *     that do not work earning ”0”.
     *
     * @return QueryResult.LowSalaryEmployees[]
     */
    public QueryResult.LowSalaryEmployees[] getLowSalaryEmployeesForDepartments() 
    {
        List<QueryResult.LowSalaryEmployees> lowSalaryEmployees = new ArrayList<>();

        try 
        {
            String query = "SELECT ge1.employeeId, ge1.employeeName, ge1.department, ge1.salary FROM (SELECT ge.department, AVG(ge.salary) AS avs FROM (SELECT ei.employeeId, ei.department, IF(ei.employeeId in (select essek.employeeId from Works_In essek), ei.salary, 0) AS salary FROM Employee ei) AS ge GROUP BY ge.department) AS ort, (SELECT ei1.employeeId, ei1.employeeName, ei1.department, IF(ei1.employeeId in (select essek1.employeeId from Works_In essek1), ei1.salary, 0) AS salary FROM Employee ei1) AS ge1 WHERE ge1.department=ort.department AND ge1.salary<ort.avs ORDER BY ge1.employeeId ASC";

            PreparedStatement st = this.connection.prepareStatement(query);
            ResultSet rs = st.executeQuery();

            while(rs.next()) 
            {
                lowSalaryEmployees.add(new QueryResult.LowSalaryEmployees(
                    rs.getInt("employeeId"),
                    rs.getString("employeeName"),
                    rs.getString("department"),
                    (int)(rs.getDouble("salary"))
                ));
            }

            // Close
            rs.close();
            st.close();
        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return lowSalaryEmployees.toArray(new QueryResult.LowSalaryEmployees[0]);    
    }


    /**
     * For the products of given productType, increase the productionCost of every unit by a given percentage.
     *
     * @return number of rows affected
     */
    public int increaseCost(String productType, double percentage) 
    {
        int numberofRowsAffected = 0;

        try 
        {
            PreparedStatement st=this.connection.prepareStatement("update Produce set productionCost=productionCost*? where productId IN(select p.productId from Product p where p.productType=?)");
            st.setString(2,productType);
            st.setDouble(1, percentage);

            numberofRowsAffected=st.executeUpdate();

            //close
            st.close();

        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return numberofRowsAffected;    
    }


    /**
     * Deleting all employees that have not worked since the given date.
     *
     * @return number of rows affected
     */
    public int deleteNotWorkingEmployees(String givenDate) 
    {
        int numberofRowsAffected = 0;

        try 
        {
            PreparedStatement st=this.connection.prepareStatement("DELETE FROM Employee WHERE employeeId NOT IN (SELECT w.employeeId FROM Works_In w where w.startDate>=? )");
            st.setDate(1, Date.valueOf(givenDate));
            numberofRowsAffected=st.executeUpdate();

            //close
            st.close();

        } 
        catch (SQLException e) 
        {
            e.printStackTrace();
        }

        return numberofRowsAffected;    
    }


    /**
     * *****************************************************
     * *****************************************************
     * *****************************************************
     * *****************************************************
     *  THE METHODS AFTER THIS LINE WILL NOT BE GRADED.
     *  YOU DON'T HAVE TO SOLVE THEM, LEAVE THEM AS IS IF YOU WANT.
     *  IF YOU HAVE ANY QUESTIONS, REACH ME VIA EMAIL.
     * *****************************************************
     * *****************************************************
     * *****************************************************
     * *****************************************************
     */

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Peers are considered tied and receive the same rank. After
     * that, there should be a gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each department, find the rank of the employees in terms of
     * salary. Everything is the same but after ties, there should be no
     * gap.
     *
     * @return QueryResult.EmployeeRank[]
     */
    public QueryResult.EmployeeRank[] calculateRank2() {
        return new QueryResult.EmployeeRank[0];
    }

    /**
     * For each factory, calculate the most profitable 4th product.
     *
     * @return QueryResult.FactoryProfit
     */
    public QueryResult.FactoryProfit calculateFourth() {
        return new QueryResult.FactoryProfit(0,0,0);
    }

    /**
     * Determine the salary variance between an employee and another
     * one who began working immediately after the first employee (by
     * startDate), for all employees.
     *
     * @return QueryResult.SalaryVariant[]
     */
    public QueryResult.SalaryVariant[] calculateVariance() {
        return new QueryResult.SalaryVariant[0];
    }

    /**
     * Create a method that is called once and whenever a Product starts
     * losing money, deletes it from Produce table
     *
     * @return void
     */
    public void deleteLosing() {

    }
}
