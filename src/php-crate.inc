<?php
namespace macropage\helper\crate;

use Toper\Client;
use Toper\GuzzleClientFactory;
use Toper\StaticHostPoolProvider;

/**
 * Class crate
 */
class crate  {

    /**
     * @var StaticHostPoolProvider
     */
	private StaticHostPoolProvider $hostPollProvider;
    private string $CrateApiUrl;

	public function __construct($connection_pool,$url) {
        $this->hostPollProvider = new StaticHostPoolProvider(
            explode(',',$connection_pool)
        );
        $this->CrateApiUrl = $url;
    }

    public function sql($sql): ?array {
        $guzzleClientFactory = new GuzzleClientFactory();
        $toper = new Client($this->hostPollProvider, $guzzleClientFactory);
        $request = $toper->post($this->CrateApiUrl);
		try {
			$request->setBody(json_encode(['stmt' => $sql], JSON_THROW_ON_ERROR));
		} catch (\JsonException $e) {
			throw new \RuntimeException($e->getMessage());
		}
		try {
            $response = $request->send();
        } catch (\Exception $e) {
            echo "Exception: ".$e."\n";
            // unable to connect to host
            if (strpos($e,'[curl] 7')!==false) {
                preg_match('/:\s(.*)\'/',$e,$matches);
                return array('state' => false, 'message' => ($matches[1]) ?: $e);
            }

			return array('state' => false, 'message' => $e);
		}
        $Cols       = array();
        $ParsedRows = array();
        if($response->getStatusCode() === 200) {
			try {
				$result = json_decode($response->getBody()->getContents(), JSON_THROW_ON_ERROR, 512, JSON_THROW_ON_ERROR);
			} catch (\JsonException $e) {
				throw new \RuntimeException($e->getMessage());
			}
			$Cols = $result['cols'];
            foreach ($result['rows'] as $i => $row) {
                $newrow = array();
                foreach ($row as $num => $value) {
					$newrow[$Cols[$num]] = $value;
                }
				$ParsedRows[$i] = $newrow;
            }
            $result['rows'] = $ParsedRows;
            unset($ParsedRows);
            return array('state' => true,'status' => $response->getStatusCode(), 'result' => $result['rows'],'numrows' => $result['rowcount'],'duration' => $result['duration']);
        }

		try {
			$result = json_decode($response->getBody()->getContents(), JSON_THROW_ON_ERROR, 512, JSON_THROW_ON_ERROR);
		} catch (\JsonException $e) {
			throw new \RuntimeException($e->getMessage());
		}

		return array('state' => false, 'status' => $response->getStatusCode(), 'message' => $result['error']['message'], 'code' => $result['error']['code']);
	}
}

