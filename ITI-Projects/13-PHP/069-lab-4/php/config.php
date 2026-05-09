<?php
define("DB_HOST", "db");
define("DB_USER", "appuser");
define("DB_PASSWORD", "apppass");
define("DB_DATABASE", "usersdb");

define("UPLOAD_DIR", __DIR__ . "/uploads/");
define("UPLOAD_URL", "/uploads/");

function get_conn(): mysqli {
    $conn = mysqli_connect(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
    if (mysqli_connect_errno()) {
        trigger_error(mysqli_connect_error());
    }
    return $conn;
}