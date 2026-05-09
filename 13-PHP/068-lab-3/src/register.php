<?php
$errors  = [];
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  if (empty($_POST['name']) || !preg_match('/^[a-zA-Z\s]{2,50}$/', $_POST['name'])) {
    $errors['name'] = "Name is required and must be 2-50 letters only.";
  }
 
  if (empty($_POST['email']) || 
  !preg_match('/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/', $_POST['email']) ||
  !filter_var($_POST['email'], FILTER_VALIDATE_EMAIL)
  ) {
      $errors['email'] = "Valid email is required.";
  }
 
  if (empty($_POST['password']) || !preg_match('/^[a-z0-9_]{8}$/', $_POST['password'])) {
      $errors['password'] = "Password does not meet requirements.";
  }
 
  if (empty($_POST['confirm']) || $_POST['confirm'] !== $_POST['password']) {
      $errors['confirm'] = "Password does not match.";
  }
 
  if (empty($_POST['room']) || !in_array($_POST['room'], ['Application 1', 'Application 2', 'Cloud'])) {
      $errors['room'] = "Room is required and must be one of vlaid options.";
  }
 
  $file_tmp = '';
  $file_name = '';
  if (empty($_FILES['picture']['name'])) {
      $errors['picture'] = 'A profile picture is required.';
  } else {
      $file_name = $_FILES['picture']['name'];
      $file_tmp = $_FILES['picture']['tmp_name'];
 
      $ext = explode('.', $file_name);
      $file_ext = strtolower(end($ext));
 
      if (!in_array($file_ext, ['jpeg', 'jpg', 'png', 'gif', 'webp'])) {
          $errors['picture'] = 'Not supported file type.';
      }
  }
 
  if (empty($errors)) {
    $uploadDir = __DIR__ . '/files/';
    move_uploaded_file($file_tmp, $uploadDir . $file_name);
    $record = implode('|', [
        $_POST['name'],
        $_POST['email'],
        md5($_POST['password']),
        $_POST['room'],
        'files/' . $file_name,
    ]);
 
    file_put_contents(__DIR__ . '/users.txt', $record.PHP_EOL, FILE_APPEND);
    header('Location: index.php');
    exit;
  }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Register</title>
  <style>* {margin:10px;} .err {color: red}</style>
</head>
<body>
  <h2>Register</h2>
  <form method="POST" enctype="multipart/form-data">
    <label>Name: <input type="text" name="name" value="<?= $_POST['name'] ?? '' ?>"></label>
    <span class="err"><?= isset($errors['name']) ? '<br>' . $errors['name'] : '' ?><br></span>

    <label>Email:<input type="text" name="email" value="<?= $_POST['email'] ?? '' ?>"></label>
    <span class="err"><?= isset($errors['email']) ? '<br>' . $errors['email'] : '' ?><br></span>

    <label>Password (8 chars, lowercase + digits + underscore only, no capitals):<input type="password" name="password"></label>
    <span class="err"><?= isset($errors['password']) ? '<br>' . $errors['password'] : '' ?><br></span>

    <label>Confirm Password:<input type="password" name="confirm"></label>
    <span class="err"><?= isset($errors['confirm']) ? '<br>' . $errors['confirm'] : '' ?><br></span>

    <label>Room Number:
      <select name="room">
        <option value="Application 1">Application 1</option>
        <option value="Application 2">Application 2</option>
        <option value="Cloud">Cloud</option>
      </select>
    </label>
    <span class="err"><?= isset($errors['room']) ? '<br>' . $errors['room'] : '' ?><br></span>

    <label>Profile Picture (JPEG/PNG/GIF/WEBP, max 2 MB):<br><input type="file" name="picture" accept="image/*"></label>
    <span class="err"><?= isset($errors['picture']) ? '<br>' . $errors['picture'] : '' ?><br></span>

    <input type="submit" value="Register">
    <input type="reset" value="Reset">
  </form>

  <p><a href="index.php">Login</a></p>
</body>
</html>