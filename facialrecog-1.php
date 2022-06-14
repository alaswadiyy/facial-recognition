<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/css/bootstrap.min.css" integrity="sha384-zCbKRCUGaJDkqS1kPbPd7TveP5iyJE0EjAuZQTgFLD2ylzuqKfdKlfG/eSrtxUkn" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdn.datatables.net/1.12.1/css/dataTables.bootstrap4.min.css">
    <link rel="stylesheet" href="https://cdn.datatables.net/1.12.1/css/jquery.dataTables.min.css">
    <link rel="stylesheet" href="https://cdn.datatables.net/buttons/2.2.3/css/buttons.dataTables.min.css">

    <title>Attendance Data</title>
</head>
<body>

  <?php
    $servername = "localhost";

    // REPLACE with your Database name
    $dbname = "database name";
    // REPLACE with Database user
    $username = "username";
    // REPLACE with Database user password
    $password = "password";

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    } 

    $sql = "SELECT id,vessel,equipment,runhours,p1,p2,p3,p4,p5,p6,reading_time FROM AUX_GEN1 ORDER BY id DESC limit 1000 offset 0";
  ?>

    <div class="container-fluid">
        <h1>Lagos State University </h1>
        <h1>Facial Recognition Student Attendance System </h1>
        <div class="table">
            <table class="table table-bordered table-striped w-100" id="datatable">
                <thead>
                <tr> 
                <td>DATA ID</td> 
                <td>UNIVERSITY</td> 
                <td>DEPARTMENT</td> 
                <td>MATRIC NUMBER</td>
                <td>NAME</td> 
                <td>PHONE NUMBER</td> 
                <td>SEX</td> 
                <td>LECTURER</td> 
                <td>COURSE</td> 
                <td>TIME</td> 
                <td>DATE ADDED</td> 
                </tr>
                </thead>
                <tbody>
                <?php
                    if ($result = $conn->query($sql)) {
                        while ($row = $result->fetch_assoc()) {
                            $row_id = $row["id"];
                            $row_vessel = $row["vessel"];
                            $row_equipment = $row["equipment"];
                            $row_runhours = $row["runhours"]; 
                            $row_p1 = $row["p1"];
                            $row_p2 = $row["p2"];
                            $row_p3 = $row["p3"];
                            $row_p4 = $row["p4"];
                            $row_p5 = $row["p5"];
                            $row_p6 = $row["p6"];
                  
                            $row_reading_time = $row["reading_time"];
                            // Uncomment to set timezone to - 1 hour (you can change 1 to any number)
                            //$row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time - 1 hours"));
                          
                            // Uncomment to set timezone to + 4 hours (you can change 4 to any number)
                            $row_reading_time = date("Y/m/d H:i:s", strtotime("$row_reading_time + 1 hours"));
                          
                            echo '<tr> 
                                    <td>' . $row_id . '</td> 
                                    <td>' . $row_vessel . '</td> 
                                    <td>' . $row_equipment . '</td> 
                                    <td>' . $row_runhours . '</td>
                                    <td>' . $row_p1 . '</td> 
                                    <td>' . $row_p2 . '</td>
                                    <td>' . $row_p3 . '</td>
                                    <td>' . $row_p4 . '</td>
                                    <td>' . $row_p5 . '</td>
                                    <td>' . $row_p6 . '</td>
                                    <td>' . $row_reading_time . '</td> 
                                  </tr>';
                        }
                        $result->free();
                    }
                  
                    $conn->close();
                ?> 
                </tbody>
            </table>
        </div>
    </div>

    <script src="https://cdn.jsdelivr.net/npm/jquery@3.5.1/dist/jquery.slim.min.js" integrity="sha384-DfXdz2htPH0lsSSs5nCTpuj/zy4C+OGpamoFVy38MVBnE+IbbVYUew+OrCXaRkfj" crossorigin="anonymous"></script>
    <script src="https://cdn.jsdelivr.net/npm/bootstrap@4.6.1/dist/js/bootstrap.bundle.min.js" integrity="sha384-fQybjgWLrvvRgtW6bFlB7jaZrFsaBXjsOMm/tB9LTS58ONXgqbR9W8oWht/amnpF" crossorigin="anonymous"></script>
    <script src="https://cdn.datatables.net/1.12.1/js/jquery.dataTables.min.js"></script>
    <script src="https://cdn.datatables.net/buttons/2.2.3/js/dataTables.buttons.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.1.3/jszip.min.js"></script>
    <script src="https://cdn.datatables.net/buttons/2.2.3/js/buttons.html5.min.js"></script>
    <script>
        $(document).ready(function() {
            $('#datatable').DataTable( {
                dom: 'Bfrtip',
                buttons: [
                    'excelHtml5'
                ]
            });
        });
    </script>
</body>
</html>