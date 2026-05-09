<?php
require_once "Database.php";

if (isset($_GET['delete'])) {
  $id = $_GET['delete'];
  $user = Database::getInstance()->selectOne('users', ['id' => $id]);
  Database::getInstance()->delete('users', $id);
  if ($user['profile_picture'] && file_exists(UPLOAD_DIR . $user['profile_picture'])) {
    unlink(UPLOAD_DIR . $user['profile_picture']);
  }
  header("Location: users.php");
  exit;
}

$users = Database::getInstance()->select('users');
?>

<!DOCTYPE html>
<html lang="en">
<head>
  <title>All Users</title>
  <style>
    * { margin: 10px; }
    img { width: 40px; height: 40px; object-fit: cover; }
    td { border: 1px solid #ccc; padding: 4px; }
  </style>
</head>
<body>
  <h2>All Users</h2>
  <a href="index.php">Register New User</a>

  <table>
    <thead>
      <tr><th>Photo</th><th>Name</th><th>Email</th><th>Room</th><th>Actions</th></tr>
    </thead>
    <tbody>
      <?php foreach ($users as $u): ?>
      <tr>
        <td>
          <?php if ($u['profile_picture'] && file_exists(UPLOAD_DIR . $u['profile_picture'])): ?>
            <img src="<?= UPLOAD_URL . $u['profile_picture'] ?>">
          <?php endif; ?>
        </td>
        <td><?= $u['name'] ?></td>
        <td><?= $u['email'] ?></td>
        <td><?= $u['room_number'] ?></td>
        <td>
          <a href="edit_user.php?id=<?= $u['id'] ?>">Edit</a>
          <a href="users.php?delete=<?= $u['id'] ?>">Delete</a>
        </td>
      </tr>
      <?php endforeach; ?>
    </tbody>
  </table>
</body>
</html>