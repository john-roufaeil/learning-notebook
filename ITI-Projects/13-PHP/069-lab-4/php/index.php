<?php
require_once "config.php";
$conn = get_conn();
$errors = [];

if ($_SERVER["REQUEST_METHOD"] === "POST") {
  $name = $_POST["name"];
  $email = $_POST["email"];
  $password = $_POST["password"];
  $confirm_pw = $_POST["confirm_password"];
  $room_number = $_POST["room_number"];

  if (empty($name)) $errors[] = "Name is required.";
  if (!filter_var($email, FILTER_VALIDATE_EMAIL)) $errors[] = "Valid email is required.";
  if (strlen($password) < 6) $errors[] = "Password must be at least 6 characters.";
  if ($password !== $confirm_pw) $errors[] = "Passwords do not match.";
  if (empty($room_number)) $errors[] = "Room number is required.";

  $pic_filename = null;
  if (isset($_FILES["profile_picture"])) {
    $file = $_FILES["profile_picture"];
    $extension = pathinfo($file['name'], PATHINFO_EXTENSION);
    $pic_filename = $name.time().".".$extension;
  }

  if (empty($errors)) {
    move_uploaded_file($file["tmp_name"], UPLOAD_DIR . $pic_filename);
    $hashed = password_hash($password, PASSWORD_BCRYPT);
    $stmt = mysqli_prepare($conn, "INSERT INTO users (name, email, password, room_number, profile_picture) VALUES (?, ?, ?, ?, ?)");
    mysqli_stmt_bind_param($stmt, "sssss", $name, $email, $hashed, $room_number, $pic_filename);
    mysqli_stmt_execute($stmt);
    mysqli_stmt_close($stmt);
    header("Location: users.php");
    exit;
  }
}
mysqli_close($conn);
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <title>Register</title>
    <style> * { margin: 10px} .errors { color: red} </style>
  </head>
  <body>
    <h2>Register</h2>

    <?php if (!empty($errors)): ?>
      <ul class="errors">
        <?php foreach ($errors as $e) echo "<li>".$e."</li>" ?>
      </ul>
    <?php endif; ?>

    <form method="POST" enctype="multipart/form-data">
      <label>Name</label><input type="text" name="name"><br>
      <label>Email</label><input type="email" name="email"><br>
      <label>Password</label><input type="password" name="password"><br>
      <label>Confirm Password</label><input type="password" name="confirm_password"><br>
      <label>Room Number</label><input type="text" name="room_number"><br>
      <label>Profile Picture</label><input type="file" name="profile_picture" accept="image/*"><br>
      <button type="submit">Register</button>
    </form>
    <a href="users.php">View users</a>
  </body>
</html>