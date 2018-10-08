<?php
	
	$flag = $_GET["flag"];

	$fp = stream_socket_client("tcp://127.0.0.1:51800", $errno, $errstr, 30);
	if (!$fp) {
	    echo "$errstr ($errno)<br />\n";
	} else {
	    fwrite($fp, 'w'.$flag);
	    while (!feof($fp)) {
	        echo fgets($fp, 1024);
	    }
	    fclose($fp);
	}
?>