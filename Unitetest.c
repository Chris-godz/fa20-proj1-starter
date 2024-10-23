#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void check_ppm_format(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("打开文件失败");
        return;
    }

    char line[256];
    int width = 0, height = 0;
    int max_color = 0;

    // 检查文件头
    if (!fgets(line, sizeof(line), file) || strcmp(line, "P3\n") != 0) {
        printf("错误：文件头不是 P3 格式\n");
        fclose(file);
        return;
    }

    // 检查注释和图像尺寸
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#') continue; // 跳过注释行

        if (width == 0 && height == 0) {
            if (sscanf(line, "%d %d", &width, &height) != 2) {
                printf("错误：无法读取图像尺寸\n");
                fclose(file);
                return;
            }
            continue;
        }

        // 检查颜色深度
        if (max_color == 0) {
            if (sscanf(line, "%d", &max_color) != 1 || max_color != 255) {
                printf("错误：颜色深度不正确\n");
                fclose(file);
                return;
            }
            continue;
        }

        // 检查像素数据
        int pixel_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL) {
            pixel_count++;
            token = strtok(NULL, " ");
        }

        if (pixel_count % 3 != 0) {
            printf("错误：像素数据不完整\n");
            fclose(file);
            return;
        }
    }

    printf("PPM 文件格式正常\n");
    fclose(file);
}

int main() {
    check_ppm_format("./studentOutputs/secretMessage.ppm");
    return 0;
}
