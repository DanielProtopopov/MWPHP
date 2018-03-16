<?php
if($argv[1] && file_exists($argv[1])) {
	echo 'ManateeWorks Library Version: ' . getLibVersion() . PHP_EOL;
	echo 'Library Codes: ' . getSupportedCodes() . PHP_EOL;
	echo 'Registration output: ' . registerSDK("t74AqrBqbxW50wE7IuM4RLaX0dh1fDemSgrZR19DPAc=") . PHP_EOL;

	setResultType(MWB_RESULT_TYPE_MW);
	setDirection(MWB_SCANDIRECTION_HORIZONTAL | MWB_SCANDIRECTION_VERTICAL);
	setActiveCodes(MWB_CODE_MASK_25    |
                           MWB_CODE_MASK_39     |
                           MWB_CODE_MASK_93     |
                           MWB_CODE_MASK_128    |
                           MWB_CODE_MASK_AZTEC  |
                           MWB_CODE_MASK_DM     |
                           MWB_CODE_MASK_EANUPC |
                           MWB_CODE_MASK_PDF    |
                           MWB_CODE_MASK_QR     |
                           MWB_CODE_MASK_CODABAR|
                           MWB_CODE_MASK_DOTCODE|
                           MWB_CODE_MASK_11     |
                           MWB_CODE_MASK_MSI    |
                           MWB_CODE_MASK_MAXICODE    |
                           MWB_CODE_MASK_POSTAL |
                           MWB_CODE_MASK_RSS);
	setLevel(3);

	// $startScan = microtime(true);
	$scan = scanGrayscaleImage(realpath($argv[1]), MWP_PARSER_MASK_AAMVA);
	// echo 'Scanning took ' . (microtime(true) - $startScan) . ' seconds' . PHP_EOL;
	print_r($scan);
}
