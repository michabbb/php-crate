<?php
/** @noinspection PhpIncludeInspection */
require_once __DIR__.'/../vendor/autoload.php';

$APIURL = "/_sql?pretty";
$POOL   = "http://127.0.0.1:4200,http://127.0.0.1:4201,http://127.0.0.1:4202";
$SQL    = "select id from test";

