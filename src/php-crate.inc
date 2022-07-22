<?php
namespace macropage\helper\crate;

use GuzzleHttp\Client;
use GuzzleHttp\Exception\GuzzleException;
use GuzzleHttp\Psr7\Request;

/**
 * Class crate
 */
class crate  {

    private string $CrateApiUrl;
	private array  $RequestOptions;

	public function __construct($url,array $request_options = []) {
        $this->CrateApiUrl = $url;
        $this->RequestOptions = $request_options;
    }

    public function sql($sql): ?array {
		try {
			$body = json_encode(['stmt' => $sql], JSON_THROW_ON_ERROR);
		} catch (\JsonException $e) {
			throw new \RuntimeException($e->getMessage());
		}
		$Client = new Client();
        $headers = (array_key_exists('headers',$this->RequestOptions)) ? $this->RequestOptions['headers'] : [];
        $Request = new Request('POST',$this->CrateApiUrl,$headers,$body);
		try {
            $response = $Client->send($Request);
        } catch (\RuntimeException $e) {
			// unable to connect to host
			if (strpos($e,'[curl] 7')!==false) {
				preg_match('/:\s(.*)\'/',$e,$matches);
				return array('state' => false, 'message' => ($matches[1]) ?: $e);
			}
			return array('state' => false, 'message' => $e);
		}
		$ParsedRows = [];
        if($response->getStatusCode() === 200) {
			try {
				$result = json_decode($response->getBody()->getContents(), JSON_THROW_ON_ERROR, 512, JSON_THROW_ON_ERROR);
			} catch (\JsonException $e) {
				throw new \RuntimeException($e->getMessage());
			}
			$Cols = $result['cols'];
            foreach ($result['rows'] as $i => $row) {
                $newrow = [];
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

