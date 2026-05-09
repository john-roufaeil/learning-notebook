<?php
define("DB_HOST", "db");
define("DB_USER", "appuser");
define("DB_PASSWORD", "apppass");
define("DB_DATABASE", "usersdb");

define("UPLOAD_DIR", __DIR__ . "/uploads/");
define("UPLOAD_URL", "/uploads/");

class Database {
  private static ? Database $instance = null;
  private mysqli $conn;

  private function __construct() {
    $this->conn = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
  }

  private function __destruct() {
    $this->conn->close();
  }
  
  public static function getInstance(): Database {
    if (Database::$instance === null) {
      Database::$instance = new Database();
    }
    return Database::$instance;
  }

  public function select(string $table): array {
    $result = $this->conn->query("SELECT * FROM $table");
    return $result->fetch_all(MYSQLI_ASSOC);
  }

  public function selectOne(string $table, array $where): ?array {
    $keys = array_keys($where);
    $col = $keys[0];
    $val = $where[$col];
    $stmt = $this->conn->prepare("SELECT * FROM $table WHERE $col = ? LIMIT 1");
    $stmt->bind_param("s", $val);
    $stmt->execute();
    $row = $stmt->get_result()->fetch_assoc();
    $stmt->close();
    return $row ?: null;
  }

  public function insert(string $table, array $data): bool {
    $cols = implode(', ', array_keys($data));
    $marks = implode(', ', array_fill(0, count($data), '?'));
    $types = str_repeat('s', count($data));
    $values = array_values($data);
    $stmt = $this->conn->prepare("INSERT INTO $table ($cols) VALUES ($marks)");
    $stmt->bind_param($types, ...$values);
    $ok = $stmt->execute();
    $stmt->close();
    return $ok;
  }

  public function update(string $table, int $id, array $data): bool {
    $set = implode(', ', array_map(fn($col) => "$col = ?", array_keys($data)));
    $types = str_repeat('s', count($data)) . 'i';
    $values = [...array_values($data), $id];
    $stmt = $this->conn->prepare("UPDATE $table SET $set WHERE id = ?");
    $stmt->bind_param($types, ...$values);
    $ok = $stmt->execute();
    $stmt->close();
    return $ok;
  }

  public function delete(string $table, int $id): bool {
    $stmt = $this->conn->prepare("DELETE FROM $table WHERE id = ?");
    $stmt->bind_param("i", $id);
    $ok = $stmt->execute();
    $stmt->close();
    return $ok;
  }
}