<?php
require_once "Database.php";
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

  $pic = null;
  if (isset($_FILES["profile_picture"])) {
    $ext = pathinfo($_FILES["profile_picture"]["name"], PATHINFO_EXTENSION);
    $pic = $name . time() . "." . $ext;
    }
    
    if (empty($errors)) {
      Database::getInstance()->insert('users', [
        'name' => $name,
        'email' => $email,
        'password' => password_hash($password, PASSWORD_BCRYPT),
        'room_number' => $room_number,
        'profile_picture' => $pic,
      ]);
      move_uploaded_file($_FILES["profile_picture"]["tmp_name"], UPLOAD_DIR . $pic);
      header("Location: users.php");
    exit;
  }
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <title>Register</title>
    <style>* { margin: 10px; } .errors { color: red; }</style>
</head>
<body>
  <h2>Register</h2>

  <?php if (!empty($errors)): ?>
    <ul class="errors">
      <?php foreach ($errors as $e) echo "<li>$e</li>"; ?>
    </ul>
  <?php endif; ?>

  <form method="POST" enctype="multipart/form-data">
    <label>Name</label><br><input type="text" name="name"><br>
    <label>Email</label><br><input type="email" name="email"><br>
    <label>Password</label><br><input type="password" name="password"><br>
    <label>Confirm Password</label><br><input type="password" name="confirm_password"><br>
    <label>Room Number</label><br><input type="text" name="room_number"><br>
    <label>Profile Picture</label><br><input type="file" name="profile_picture" accept="image/*"><br>
    <button type="submit">Register</button>
  </form>
  <a href="users.php">View Users</a>
</body>
</html>