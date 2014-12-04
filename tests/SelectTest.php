<?php
namespace macropage\helper\crate;

class SelectTest extends \PHPUnit_Framework_TestCase {

    /**
     * @outputBuffering enabled
     */
    public function test_select() {
        global $POOL,$APIURL,$SQL;
        $CRATE = new php_crate($POOL,$APIURL);
        $result = $CRATE->sql($SQL);
        print_r($result);
        $this->assertTrue($result['state']);
    }

}