<?php
// $Id 

require_once "Twitter.php";

class Arc90_Service_Twitter_Search extends Arc90_Service_Twitter
{
	const API_URL					= 'http://search.twitter.com';
	const PATH_ATOM_BASE			= '/search.atom?q=';
	const PATH_JSON_BASE			= '/search.json?q=';
	const PATH_FROM_USER			= 'from%3A';
	const OR_DELIMITER			= '+OR+';

	public function __construct() {
		parent::__construct('', '');
	}//__construct()

	public function __destruct() {
		//parent::__destruct();
	}//__destruct()

	public function getTweetsForUsers(
		$users = NULL, $format = 'json', 
		$since_id = 0, $page = 0) {
			
		if(!is_array($users)) {
			//throw an exception
			throw new Arc90_Service_Twitter_Exception(
				"Invalid parameter (users). Must be an array."
			);
		}
		
		$requestedFormat = strtolower($format);
		$validFormats    = array('json', 'atom');
		$this->_validateOption($requestedFormat, $validFormats);

		//build the search string
		$url = ($requestedFormat == 'json') ? self::PATH_JSON_BASE : self::PATH_ATOM_BASE;

		$first = 0;
		foreach ($users as $user) {
			$first ? $url .= self::OR_DELIMITER : $first++;
			$url .= self::PATH_FROM_USER . $user;
		}//foreach

		if(0 !== $since_id)
		{
			$this->_validateNonNegativeInteger('since_id', $since_id);
			$url .= "&since_id=$since_id";
		}

		//we don't want to get limited by rpp
		$url .= "&rpp=100";

		return $this->_makeRequest($url, $requestedFormat);	

	}//getTweetsForUsers

	/**
	 * We have to overide this function from the base class because of the 
	 * reference to self::API_URL
	 *
	 * Sends an HTTP GET or POST request to the Twitter API with optional Basic authentication.
	 *
	 * @param  string $url    Target URL for this request (relative to the API root URL)
	 * @param  string $format Response format (JSON,XML, RSS, ATOM, none); Defaults to JSON
	 * @param  bool   $auth   Specifies whether authentication is required
	 * @param  bool   $post   Specifies whether HTTP POST should be used (versus GET)
	 * @param  array  $data   x-www-form-urlencoded data to be sent in a POST request body
	 * @return Arc90_Service_Twitter_Response
	 * @throws Arc90_Service_Twitter_Exception
	 */
	protected function _makeRequest($url, $format ='json', $auth =false, $post =false, $data ='')
	{
		$ch = curl_init();

		curl_setopt($ch, CURLOPT_URL, self::API_URL . $url);
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);

		if($auth)
		{
			curl_setopt($ch, CURLOPT_USERPWD, "{$this->_authUsername}:{$this->_authPassword}");
		}

		if($post)
		{
			curl_setopt($ch, CURLOPT_POST, true);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $data);
		}

		$this->_lastRequestTime = time();

		$data     = curl_exec($ch);
		$metadata = curl_getinfo($ch);

		curl_close($ch);

		// Create, store and return a response object
		return $this->_lastResponse = new Arc90_Service_Twitter_Response($data, $metadata, $format);
	}//_makeRequest
	

}

