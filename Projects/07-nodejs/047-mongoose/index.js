import js from "@eslint/js";
import globals from "globals";
import json from "@eslint/json";
import { defineConfig } from "eslint/config";

export default defineConfig([
    js.configs.recommended,
    {
        files: ["**/*.js"],
        languageOptions: {
            sourceType: "commonjs",
            globals: globals.node
        },
        rules: {
            // "no-console": "warn",
            "prefer-const": "error",
            "no-unused-vars": "warn",
            "no-var": "error",
            "eqeqeq": "error",
            "max-len": ["error", { "code": 80, "tabWidth": 4 }]
        }
    },
    {
        files: ["**/*.json"],
        plugins: { json },
        language: "json/json"
    },
    {
        ignores: ["node_modules/**", "dist/**"]
    }
]);
