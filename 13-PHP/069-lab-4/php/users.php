<?php
require_once "config.php";
$conn = get_conn();

if (isset($_GET['delete'])) {
  $id = $_GET['delete'];
  $result = mysqli_query($conn, "SELECT profile_picture FROM users WHERE id = $id");
  $user = mysqli_fetch_assoc($result);

  $stmt = mysqli_prepare($conn, "DELETE FROM users WHERE id = ?");
  mysqli_stmt_bind_param($stmt, "i", $id);
  mysqli_stmt_execute($stmt);
  mysqli_stmt_close($stmt);
  if ($user['profile_picture'] && file_exists(UPLOAD_DIR . $user['profile_picture'])) {
    unlink(UPLOAD_DIR . $user['profile_picture']);
  }

  header("Location: users.php");
  exit;
}
 
$result = mysqli_query($conn, "SELECT * FROM users");
$users  = [];
while ($row = mysqli_fetch_assoc($result)) {
  $users[] = $row;
}
mysqli_free_result($result);
mysqli_close($conn);
?>

<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>All Users</title>
<style>
  * { margin: 10px; }
  img { width: 40px; height: 40px; object-fit: cover; }
  td {border: 1px solid #ccc;}
</style>
</head>
<body>

<h2>All Users </h2>
<a href="index.php">New User</a>

<table>
  <thead>
    <tr><th>Photo</th><th>Name</th><th>Email</th><th>Room</th><th>Actions</th></tr>
  </thead>
  <tbody>
    <?php foreach ($users as $u): ?>
    <tr>
      <td>
        <?php if ($u['profile_picture'] && file_exists(UPLOAD_DIR . $u['profile_picture'])) {
          echo "<img src=\"" . UPLOAD_URL . $u['profile_picture'] . "\">";
        } ?>
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