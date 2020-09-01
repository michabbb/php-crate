<?php /** @noinspection PhpIllegalPsrClassPathInspection */

namespace macropage\helper\crate;

use PHPUnit\Framework\TestCase;

class SelectTest extends TestCase {

    /**
     * @outputBuffering enabled
     */
    public function test_select(): void {
        global $POOL,$APIURL,$SQL;
        $CRATE = new crate($POOL,$APIURL);
        $result = $CRATE->sql($SQL);
        print_r($result);
        $this->assertTrue($result['state']);
    }

}
