<?php
if (isset($_POST["delete_index"])) {
    $file = file("students.txt", FILE_IGNORE_NEW_LINES);
    unset($file[$_POST["delete_index"]]);
    file_put_contents("students.txt", implode(PHP_EOL, $file));
    header("Location: records.php");
    exit;
}
?>

<!DOCTYPE html>
<html>
<head>
    <title>Students Table</title>
    <style>
        body { font-family: Arial; }
        table {
            border-collapse: collapse;
            width: 100%;
            margin-top: 20px;
        }
        th, td {
            border: 1px solid #000;
            padding: 8px;
            text-align: left;
        }
        th {
            background-color: #eee;
        }
    </style>
</head>
<body>
    <h2>Students Data</h2>
    <table>
        <thead>
            <tr>
                <th>First Name</th>
                <th>Last Name</th>
                <th>Email</th>
                <th>Address</th>
                <th>Country</th>
                <th>Gender</th>
                <th>Skills</th>
                <th>Department</th>
                <th>Action</th>
            </tr>
        </thead>
        <tbody>
        <?php
          $file = file("students.txt", FILE_IGNORE_NEW_LINES);
          foreach($file as $index => $line) {
            if (empty($line)) continue;
            $data = explode("|", $line);
            echo "<tr>";
            echo "<td>{$data[0]}</td>";
            echo "<td>{$data[1]}</td>";
            echo "<td>{$data[2]}</td>";
            echo "<td>{$data[3]}</td>";
            echo "<td>{$data[4]}</td>";
            echo "<td>{$data[5]}</td>";
            echo "<td>{$data[6]}</td>";
            echo "<td>{$data[7]}</td>";
            echo "
              <td>
                <form method='POST'>
                  <input type='hidden' name='delete_index' value='$index'>
                  <button type='submit'>Delete</button>
                </form>
              </td>
            ";
            echo "</tr>";
          }
        ?>
        </tbody>
    </table>
    <a href="index.php">Back to form</a>
</body>
</html>

<!--// <a href="/records.php?id=$index">
// Delete
// </a>
-->