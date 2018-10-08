<?php

	$json = $_GET['json'];
	$archivo = $_GET['archivo'];

	if ($json == 'true') {
		echo trim(str_replace('\'', '"', file_get_contents($archivo)));
	}
	else {
		echo '[' , preg_replace('/\s+/', ',', trim(file_get_contents($archivo))) , ']';
	}
?>
