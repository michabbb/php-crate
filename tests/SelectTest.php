<?php /** @noinspection PhpIllegalPsrClassPathInspection */

namespace macropage\helper\crate;

use PHPUnit\Framework\TestCase;

class SelectTest extends TestCase {

    /**
     * @outputBuffering enabled
     */
    public function test_select(): void {
        global $APIURL,$SQL;
        $CRATE = new crate($APIURL);
        $result = $CRATE->sql($SQL);
        print_r($result);
        $this->assertTrue($result['state']);
    }

}
