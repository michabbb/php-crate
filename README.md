php-crate
=========

A PHP helper class for working with the crate db

Quick start
-----------

```php
$APIURL = "/_sql?pretty";
$POOL   = "http://127.0.0.1:4200,http://127.0.0.1:4201,http://127.0.0.1:4202";
$SQL    = "select id from test";

$CRATE = new php_crate($POOL,$APIURL);
$result = $CRATE->sql($SQL);
```

Simple Test
-----------
```shell
vendor/bin/phpunit --bootstrap ./tests/bootstrap.php --colors tests/SelectTest.php --verbose --debug
```