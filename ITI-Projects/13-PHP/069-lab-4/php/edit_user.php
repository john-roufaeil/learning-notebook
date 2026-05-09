<?php
require_once "config.php";
$conn = get_conn();

$id = $_GET['id'];
$res  = mysqli_query($conn, "SELECT * FROM users WHERE id = $id");
$user = mysqli_fetch_assoc($res);
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
    if (isset($_FILES['profile_picture']) && $_FILES['profile_picture']['error'] !== UPLOAD_ERR_NO_FILE) {
        $file = $_FILES['profile_picture'];
        $extension = pathinfo($file['name'], PATHINFO_EXTENSION);
        $pic = $name . time() . "." . $extension;
        move_uploaded_file($file['tmp_name'], UPLOAD_DIR . $pic);
    }

    if (empty($errors)) {
        $hashed = !empty($password) ? password_hash($password, PASSWORD_BCRYPT) : $user['password'];
        $stmt = mysqli_prepare($conn, "UPDATE users SET name=?, email=?, room_number=?, password=?, profile_picture=? WHERE id=?");
        mysqli_stmt_bind_param($stmt, "sssssi", $name, $email, $room_number, $hashed, $pic, $id);
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
    <title>Edit User</title>
    <style> * { margin: 10px; } img { width: 40px; height: 40px; object-fit: cover; } .errors { color: red} </style>
</head>
<body>
    <h2>Edit User</h2>
    <?php if (!empty($errors)): ?>
    <ul class="errors">
        <?php foreach ($errors as $e): echo "<li>" . $e . "</li>"; endforeach; ?>
    </ul>
    <?php endif; ?>

    <form method="POST" enctype="multipart/form-data">
    <input type="hidden" name="id" value="<?= $id ?>">

    <label>Name</label>
    <input type="text" name="name" value="<?= $_POST['name'] ?? $user['name'] ?>"><br>

    <label>Email</label>
    <input type="email" name="email" value="<?= $_POST['email'] ?? $user['email'] ?>"><br>

    <label>Room Number</label>
    <input type="text" name="room_number" value="<?= $_POST['room_number'] ?? $user['room_number'] ?>"><br>

    <label>New Password</label>
    <input type="password" name="password"><br>

    <label>Profile Picture</label>
    <input type="file" name="profile_picture" accept="image/*"><br>

    <button type="submit">Save</button>
    <a href="users.php">Cancel</a>
    </form>
</body>
</html>