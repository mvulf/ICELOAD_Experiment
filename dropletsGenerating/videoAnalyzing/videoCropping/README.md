## Сrop generating-droplets videos
- **Create video list** *"video_list.txt"* for Cropping *('0000_photron_avi.avi'-format)*: `ToCropList_creator.ipynb`; 
- **Load&crop videos** from *"video_list.txt"*: `VideoList_cropper.mlx`;
- **Write settings and results** in *"..._cropping results.yml"*, taken from `template_cropping results.yml`;
- **Create recrop list** *"toCrop_list.txt"*: `Yaml_analyzer.ipynb`;
- **Recrop videos**: 
    - **List recroping** from *"toCrop_list.txt"*: `VideoList_cropper.mlx`
    - **Single recroping**: `CropVideo_editor.mlx`;
- **Save croped videos** in *"VIDEO_DATA/CROPPED_VIDEO"* according to *"save_video.txt"*: `CropVideo_saver.mlx`

### Return in 'VIDEO_DATA'
- Template and EMD-results *"0000_template.jpg", "0000_EMD.fig"*;
- Frame ranges *"cropping_video.csv"*;
- Cropped videos *"CROPPED_VIDEO/0000_photron_0.avi"*.