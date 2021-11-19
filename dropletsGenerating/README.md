## Algorithm of a generating droplets data analyzing 
- **Get data** from generating protocol. **Add data** to *"GENERATING_DATA.csv"* **and videofiles** to *"VIDEO_DATA"* with renaming tests: `dataframeAnalyzing/Protocol_retriever.ipynb`.

- **Get pixel size**: `videoAnalyzing/Calibration_analyzer.mlx`.

- **Сrop videos**. Returns in **'VIDEO_DATA'**: 
    - "0000_template.jpg";
    - "CROPPED_VIDEO/0000_photron_0.avi".
    
    See details in `videoAnalyzing/videoCropping/README.md`.

- **Analyze droplets**. Returns in **'IMAGE_PROCESSING_DATA'**:
    - *"image_processing.csv"*;
    - *"0000_pixprop_0.csv"*;

    See details in `videoAnalyzing/dropletAnalyzing/README.md`.
- **Append all imgproc-files** *"0000_pixprop_0.csv"* in one df, then **Modify and save** this df *"imgproc_detailed.csv"*:
`dataframeAnalyzing/ImgProc_analyzer.ipynb`.

- **Add columns** from *"image_processing.csv" & "imgproc_detailed.csv"* to main generating dataframe and **get median values**. Save *"GENERATING_DATA.csv" & "GENERATING_DATA.xlsx"*:
    `dataframeAnalyzing/ImgProc_analyzer.ipynb`.
