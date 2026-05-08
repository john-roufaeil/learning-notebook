<?php
$error = '';
$loggedIn = false;
$loggedName = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
  $email = trim($_POST['email'] ?? '');
  $pw = $_POST['password']   ?? '';

  if ($email === '' || $pw === '') {
    $error = 'Please fill in both fields.';
  } else {
    $lines = file('users.txt', FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    $found = false;

    foreach ($lines as $line) {
      [$dbName, $dbEmail, $dbPw] = explode('|', $line);
      if (
        strtolower($dbEmail) === strtolower($email) &&
        md5($pw) === $dbPw
      ) {
        $found = true;
        $loggedIn = true;
        $loggedName=  $dbName;
        break;
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
  <?php if ($loggedIn): ?>
    <p>Welcome, <?= $loggedName ?></p>
    <p><a href="index.php">Logout</a></p>

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