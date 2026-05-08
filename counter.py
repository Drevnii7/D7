#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys

# Папки, которые нужно игнорировать
IGNORED_DIRS = {
    'Plugins\\AdvancedSessions',
    'Plugins\\DrevniiNoise',
    'Plugins\\MultiTaskTwo',
    'Plugins\\ColorWheelPlugin',
    'Plugins\\FlatNodes',
    'Plugins\\RestartEditor',
    'Plugins\\VisualStudioTools',
    'Intermediate\\Build'
    # Также добавим с прямыми слешами для кросс-платформенности
    'Plugins/AdvancedSessions',
    'Plugins/DrevniiNoise',
    'Plugins/MultiTaskTwo',
    'Plugins/ColorWheelPlugin',
    'Plugins/FlatNodes',
    'Plugins/RestartEditor',
    'Plugins/VisualStudioTools',
    'Intermediate/Build'
}

def should_ignore_dir(dirpath):
    """
    Проверяет, нужно ли игнорировать папку
    """
    # Проверяем полный путь и отдельные компоненты
    path_parts = dirpath.split(os.sep)
    
    for ignored in IGNORED_DIRS:
        # Проверяем, содержится ли игнорируемая папка в пути
        ignored_parts = ignored.split('/') if '/' in ignored else ignored.split('\\')
        
        # Проверяем все возможные комбинации путей
        for i in range(len(path_parts) - len(ignored_parts) + 1):
            if path_parts[i:i+len(ignored_parts)] == ignored_parts:
                return True
    
    return False

def should_ignore_file(filename):
    """
    Проверяет, нужно ли игнорировать файл
    """
    ignore_patterns = ('.generated.h', '.gen.cpp')
    return filename.endswith(ignore_patterns)

def count_files(directory):
    """
    Рекурсивно подсчитывает строки и символы в .cpp и .h файлах,
    игнорируя сгенерированные файлы и указанные папки
    """
    total_lines = 0
    total_chars = 0
    files_processed = 0
    files_ignored = 0
    dirs_ignored = 0
    
    # Расширения файлов для поиска
    extensions = ('.cpp', '.h', '.hpp', '.cxx', '.cc')
    
    # Проходим по всем файлам и папкам
    for root, dirs, files in os.walk(directory):
        # Модифицируем список dirs на месте, чтобы пропустить игнорируемые папки
        dirs_to_remove = []
        for d in dirs:
            full_path = os.path.join(root, d)
            if should_ignore_dir(full_path):
                dirs_to_remove.append(d)
                dirs_ignored += 1
                print(f"  [ИГНОР ПАПКА] {full_path}")
        
        # Удаляем игнорируемые папки из обхода
        for d in dirs_to_remove:
            dirs.remove(d)
        
        for file in files:
            # Проверяем, нужно ли игнорировать файл
            if should_ignore_file(file):
                files_ignored += 1
                print(f"  [ИГНОР ФАЙЛ] {os.path.join(root, file)}")
                continue
            
            if file.endswith(extensions):
                filepath = os.path.join(root, file)
                try:
                    with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                        content = f.read()
                        lines = content.count('\n')
                        # Если файл не пустой и не заканчивается на \n, добавляем последнюю строку
                        if content and not content.endswith('\n'):
                            lines += 1
                        chars = len(content)
                        
                        total_lines += lines
                        total_chars += chars
                        files_processed += 1
                        
                        print(f"  {filepath}: {lines} строк, {chars} символов")
                        
                except Exception as e:
                    print(f"  Ошибка при чтении {filepath}: {e}")
    
    return files_processed, files_ignored, dirs_ignored, total_lines, total_chars

def main():
    # Определяем стартовую директорию
    if len(sys.argv) > 1:
        start_dir = sys.argv[1]
    else:
        start_dir = os.getcwd()
    
    # Проверяем существование директории
    if not os.path.exists(start_dir):
        print(f"Ошибка: Директория '{start_dir}' не существует!")
        sys.exit(1)
    
    if not os.path.isdir(start_dir):
        print(f"Ошибка: '{start_dir}' не является директорией!")
        sys.exit(1)
    
    print(f"Подсчет файлов в директории: {start_dir}")
    print("Игнорируются файлы: *.generated.h, *.gen.cpp")
    print("Игнорируются папки:")
    for ignored_dir in sorted(set(IGNORED_DIRS)):
        print(f"  - {ignored_dir}")
    print()
    
    # Подсчитываем файлы
    files_count, ignored_files, ignored_dirs, lines_count, chars_count = count_files(start_dir)
    
    # Выводим итоги
    print("\n" + "="*50)
    print(f"ИТОГО:")
    print(f"  Обработано файлов: {files_count}")
    print(f"  Игнорировано файлов: {ignored_files}")
    print(f"  Игнорировано папок: {ignored_dirs}")
    print(f"  Всего строк: {lines_count}")
    print(f"  Всего символов: {chars_count}")
    
    if files_count > 0:
        print(f"\n  Среднее строк на файл: {lines_count / files_count:.1f}")
        print(f"  Среднее символов на файл: {chars_count / files_count:.1f}")

if __name__ == "__main__":
    main()