<?php
namespace macropage\helper\crate;

use Toper\Client;
use Toper\GuzzleClientFactory;
use Toper\StaticHostPoolProvider;

/**
 * Class php_crate
 */
class php_crate  {

    /**
     * @var StaticHostPoolProvider
     */
    private $hostPollProvider;
    private $CrateApiUrl;
    private $Cols = array();
    private $ParsedRows = array();

    public function __construct($connection_pool,$url) {
        $this->hostPollProvider = new StaticHostPoolProvider(
            explode(',',$connection_pool)
        );
        $this->CrateApiUrl = $url;
    }

    public function sql($sql) {
        $guzzleClientFactory = new GuzzleClientFactory();
        $toper = new Client($this->hostPollProvider, $guzzleClientFactory);
        $request = $toper->post($this->CrateApiUrl);
        $request->setBody(json_encode(array('stmt' => $sql)));
        try {
            $response = $request->send();
        } catch (\Exception $e) {
            echo "Exception: ".$e."\n";
            // unable to connect to host
            if (strpos($e,'[curl] 7')!==false) {
                preg_match('/:\s(.*)\'/',$e,$matches);
                return array('state' => false, 'message' => ($matches[1]) ? $matches[1] : $e);
            } else {
                return array('state' => false, 'message' => $e);
            }
        }
        $this->Cols = array();
        $this->ParsedRows = array();
        if($response->getStatusCode() == 200) {
            $result = $response->json();
            $this->Cols = $result['cols'];
            foreach ($result['rows'] as $i => $row) {
                array_walk($row, function (&$v, $k,$i) {
                    global $row;
                    $row[$this->Cols[$k]]=$v;
                    $this->ParsedRows[$i] = $row;
                },$i);
            }
            $result['rows'] = $this->ParsedRows;
            unset($this->ParsedRows);
            return array('state' => true,'status' => $response->getStatusCode(), 'result' => $result['rows'],'numrows' => $result['rowcount'],'duration' => $result['duration']);
        } else {
            $result = $response->json();
            return array('state' => false,'status' => $response->getStatusCode(), 'message' => $result['error']['message'],'code' => $result['error']['code']);
        }
    }
}

