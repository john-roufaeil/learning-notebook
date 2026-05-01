<?php
$data = $_POST;
var_dump($data);
$fullName = $data['firstname']." ".$data['lastname'];
$address = $data['address'].", ".$data['country'];
$gender = isset($data['gender']) ? $data['gender'] : '';
$title = ($gender === 'Male') ? 'Mr.' : 'Miss';
$skills = isset($data['skills']) ? implode(", ", $data['skills']) : "None selected";
?>

<!DOCTYPE html>
<html>
<head>
    <title>Review</title>
    <style>* {margin-top: 10px}</style>
</head>
<body>
    <?= "Thanks ".$title." ".($fullName)."!"; ?>
    <p>Please Review Your Information</p>
    <hr>
    <div>
        <div>Name: <?= $fullName; ?></div>
        <div>Address: <?= $address; ?></div>
        <div>Your Skils: <?= $skills; ?></div>
        <div>Username: <?= $data['username']; ?></div>
        <div>Department: <?= $data['department']; ?></div>
    </div>
    <a href="index.php">Go back to form</a>
</body>
</html>