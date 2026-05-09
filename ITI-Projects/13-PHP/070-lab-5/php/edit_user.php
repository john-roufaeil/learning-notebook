<?php
require_once "Database.php";

$id = $_GET['id'];
$user = Database::getInstance()->selectOne('users', ['id' => $id]);
$errors = [];

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $name = $_POST['name'];
  $email = $_POST['email'];
  $room_number = $_POST['room_number'];
  $password = $_POST['password'];

  if (empty($name)) $errors[] = "Name is required.";
  if (!filter_var($email, FILTER_VALIDATE_EMAIL)) $errors[] = "Valid email is required.";
  if (empty($room_number)) $errors[] = "Room number is required.";
  if (!empty($password) && strlen($password) < 6) $errors[] = "Password must be at least 6 characters.";

  $pic = $user['profile_picture'];
  if (!empty($_FILES['profile_picture']['name'])) {
    $ext = pathinfo($_FILES['profile_picture']['name'], PATHINFO_EXTENSION);
    $pic = $name . time() . "." . $ext;
    move_uploaded_file($_FILES['profile_picture']['tmp_name'], UPLOAD_DIR . $pic);
  }

  if (empty($errors)) {
    Database::getInstance()->update('users', $id, [
      'name' => $name,
      'email' => $email,
      'room_number' => $room_number,
      'password' => !empty($password) ? password_hash($password, PASSWORD_BCRYPT) : $user['password'],
      'profile_picture' => $pic,
    ]);
    header("Location: users.php");
    exit;
  }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <title>Edit User</title>
  <style>* { margin: 10px; } img { width: 60px; height: 60px; object-fit: cover; } .errors { color: red; }</style>
</head>
<body>
  <h2>Edit User</h2>

  <?php if ($user['profile_picture'] && file_exists(UPLOAD_DIR . $user['profile_picture'])): ?>
    <img src="<?= UPLOAD_URL . $user['profile_picture'] ?>"><br>
  <?php endif; ?>

  <?php if (!empty($errors)): ?>
    <ul class="errors">
      <?php foreach ($errors as $e) echo "<li>$e</li>"; ?>
    </ul>
  <?php endif; ?>

  <form method="POST" enctype="multipart/form-data">
    <label>Name</label><br>
    <input type="text" name="name" value="<?= $_POST['name'] ?? $user['name'] ?>"><br>

    <label>Email</label><br>
    <input type="email" name="email" value="<?= $_POST['email'] ?? $user['email'] ?>"><br>

    <label>Room Number</label><br>
    <input type="text" name="room_number" value="<?= $_POST['room_number'] ?? $user['room_number'] ?>"><br>

    <label>New Password <small>(leave blank to keep current)</small></label><br>
    <input type="password" name="password"><br>

    <label>Profile Picture <small>(leave blank to keep current)</small></label><br>
    <input type="file" name="profile_picture" accept="image/*"><br>

    <button type="submit">Save</button>
    <a href="users.php">Cancel</a>
  </form>
</body>
</html>