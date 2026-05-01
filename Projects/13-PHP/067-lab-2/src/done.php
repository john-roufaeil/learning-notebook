<?php
$data = $_POST;
$errors = [];

if (empty($data['firstname']) || !preg_match('/^[a-zA-Z\s]+$/', $data['firstname'])) {
    $errors[] = "First name is required and can only contain letters.";
}
if (empty($data['lastname']) || !preg_match('/^[a-zA-Z\s]+$/', $data['lastname'])) {
    $errors[] = "Last name is required and can only contain letters.";
}
if (empty($data['email']) || !preg_match('/^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$/', $data['email'])) {
    $errors[] = "Valid email is required.";
}
if (empty($data['gender']) || !in_array($data['gender'], ['Male', 'Female'])) {
    $errors[] = "Gender is required.";
}
if (empty($data['confirm']) || $data['confirm'] !== 'sh680c') {
    $errors[] = "Verification code is incorrect.";
}
if (!empty($errors)) {
    echo "<h2>Errors</h2><ul>";
    foreach ($errors as $err) echo "<li>$err</li>";
    echo "</ul><a href='index.php'>Go back to form</a>";
    exit;
}

$fullName = $data['firstname']." ".$data['lastname'];
$address = $data['address'].", ".$data['country'];
$gender = $data['gender'];
$title = ($gender === 'Male') ? 'Mr.' : 'Miss';
$skills = isset($data['skills']) ? implode(", ", $data['skills']) : "None selected";

$record = implode('|', [
    $data['firstname'],
    $data['lastname'],
    $data['email'],
    $data['address'],
    $data['country'],
    $data['gender'],
    $skills,
    $data['department']
]).PHP_EOL;
$file_handler = fopen("students.txt", "a");
fwrite($file_handler, $record);
fclose($file_handler);
?>
<!DOCTYPE html>
<html>
<head>
    <title>Review</title>
</head>
<body>
    <?= "Thanks ".$title." ".($fullName)."!"; ?>
    <p>Please Review Your Information</p>
    <hr>
    <div>
        <div>Name: <?= $fullName; ?></div>
        <div>Email: <?= $data['email']; ?></div>
        <div>Address: <?= $address; ?></div>
        <div>Your Skills: <?= $skills; ?></div>
        <div>Department: <?= $data['department']; ?></div>
    </div>
    <p><strong>Record saved successfully!</strong></p>
    <a href="index.php">Go back to form</a> or <a href="records.php">View all records</a>
</body>
</html>