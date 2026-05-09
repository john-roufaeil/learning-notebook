<!DOCTYPE html>
<html>
<head>
    <title>Registration Form</title>
    <style>* { margin-top: 10px; }</style>
</head>
<body>
    <h2>Registration Form</h2>
    <form action="done.php" method="POST">
        <label>First Name:</label> <input type="text" name="firstname" required><br>
        <label>Last Name:</label> <input type="text" name="lastname" required><br>
        <label>Address:</label> <textarea name="address" rows="2" cols="30"></textarea><br>
        <label>Country:</label> 
        <select name="country">
            <option>Egypt</option><option>Germany</option><option>Saudi</option><option>Other</option>
        </select><br>
        <label>Gender:</label> 
        <input type="radio" name="gender" value="Male" required> Male 
        <input type="radio" name="gender" value="Female"> Female <br>
        <label>Skills:</label>
        <div>
            <input type="checkbox" name="skills[]" value="PHP"> PHP </input>
            <input type="checkbox" name="skills[]" value="JavaScript"> JavaScript </input>
            <input type="checkbox" name="skills[]" value="Docker"> Docker </input>
            <input type="checkbox" name="skills[]" value="MySQL"> MySQL </input>
        </div><br>
        <label>Username:</label> <input type="text" name="username" required><br>
        <label>Password:</label> <input type="password" name="password" required><br>
        <label>Department:</label> <input type="text" name="department" value="Open Source" readonly><br>
        <label>Please insert the code into the box (sh680c):</label> <input type="text" name="confirm" required><br>
        <button type="submit">Register</button>
        <button type="reset">Reset</button>
    </form>
</body>
</html>