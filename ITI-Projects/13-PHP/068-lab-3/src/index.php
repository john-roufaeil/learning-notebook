<?php
session_start();

if (isset($_GET['logout'])) {
  $_SESSION = [];
  session_destroy();
  header('Location: index.php');
  exit;
}
 
$error = '';
if (isset($_SESSION['name'])) {
  $loggedName = $_SESSION['name'];
}

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $email = trim($_POST['email'] ?? '');
  $pw = $_POST['password'] ?? '';
 
  if ($email === '' || $pw === '') {
    $error = 'Please fill in both fields.';
  } else {
    $lines = file(__DIR__ . '/users.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    $found = false;
 
    foreach ($lines as $line) {
      [$dbName, $dbEmail, $dbPw] = explode('|', $line);
      if (
        strtolower($dbEmail) === strtolower($email) &&
        md5($pw) === $dbPw
      ) {
        $found = true;
        $_SESSION['name'] = $dbName;
        $_SESSION['email']    = $dbEmail;
        header('Location: index.php');
        exit;
      }
    }
 
    if (!$found) {
      $error = 'Invalid email or password.';
    }
  }
}
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Login</title>
  <style>* {margin:10px;} .err {color: red}</style>
</head>
<body>
  <h2>Login</h2>
  <?php if (isset($_SESSION['name'])): ?>
    <p>Welcome, <?= $_SESSION['name'] ?></p>
    <p><a href="index.php?logout=1">Logout</a></p>

  <?php else: ?>
    <?php if ($error): ?>
      <p class="err"><?= $error ?></p>
    <?php endif; ?>

    <form method="POST">
      <label>Email:<input type="text" name="email" value="<?= $_POST['email'] ?? '' ?>"></label><br>
      <label>Password:<input type="password" name="password"></label><br>
      <input type="submit">
    </form>
    <p><a href="register.php">Register</a></p>
  <?php endif; ?>
</body>
</html>