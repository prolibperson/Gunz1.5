directory = getSavePath caption:"미니맵 폴더 지정"
if directory != undefined then
(
    outputFileName = directory + "\\result.txt"
    outputFile = createfile outputFileName
	findFilter = directory +"\\minimap*.max"
    maxFiles = getFiles findFilter
    for f in maxFiles do
    (
        loadMAXFile f useFileUnits:true quiet:true
        format "MAX_FILE_NAME=%\n" maxFileName to: outputFile
		for n = 1 to viewport.numViews do
		(
			obj = viewport.getCamera index:n
			if obj != undefined and classOf obj == Targetcamera then
			(
				r1 = obj.transform.row1
				r2 = obj.transform.row2
				r3 = obj.transform.row3
				r4 = obj.transform.row4
				
				obj.fovType = 2
				
				format "CAMERA_NAME=%\n" obj.name to: outputFile
			--	format "MATRIX=% % % % % % % % % % % %\n" r1.x r1.y r1.z r2.x r2.y r2.z r3.x r3.y r3.z r4.x r4.y r4.z to: outputFile
				format "CAM_EYE_X=%\n" obj.position.x to: outputFile
				format "CAM_EYE_Y=%\n" obj.position.y to: outputFile
				format "CAM_EYE_Z=%\n" obj.position.z to: outputFile
				format "CAM_AT_X=%\n" obj.target.position.x to: outputFile
				format "CAM_AT_Y=%\n" obj.target.position.y to: outputFile
				format "CAM_AT_Z=%\n" obj.target.position.z to: outputFile
				format "CAM_FOV_Y=%\n" obj.curFov to: outputFile
			--	format "NEAR=%\n" obj.nearClip to: outputFile
			--	format "FAR=%\n" obj.farClip to: outputFile
			)
		)
		format "\n" to: outputFile
    )
	resetMaxFile #noPrompt
	close outputFile
)

