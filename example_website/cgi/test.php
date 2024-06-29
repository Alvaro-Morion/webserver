#!/usr/bin/php
<!-- process_form.php -->
<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $name = $_POST['name'];
    $email = $_POST['email'];
    
    echo "Name: " . $name . "<br>";
    echo "Email: " . $email;
}
?>

