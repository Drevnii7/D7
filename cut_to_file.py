#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import sys
from pathlib import Path

def collect_source_files(root_dir):
    """Рекурсивно собирает все .cpp и .h файлы в директории"""
    cpp_files = []
    h_files = []
    
    # Проходим по всем файлам в директории и поддиректориях
    for root, dirs, files in os.walk(root_dir):
        # Исключаем скрытые папки и системные (опционально)
        dirs[:] = [d for d in dirs if not d.startswith('.') and d not in ['__pycache__']]
        
        for file in files:
            if file.endswith('.cpp'):
                cpp_files.append(os.path.join(root, file))
            elif file.endswith('.h'):
                h_files.append(os.path.join(root, file))
    
    # Сортируем файлы для более предсказуемого порядка
    cpp_files.sort()
    h_files.sort()
    
    return h_files + cpp_files  # Сначала .h, потом .cpp

def merge_files(output_file, root_dir):
    """Объединяет все файлы в один с указанием источника"""
    
    source_files = collect_source_files(root_dir)
    
    if not source_files:
        print("Не найдено ни одного .cpp или .h файла!")
        return False
    
    try:
        with open(output_file, 'w', encoding='utf-8') as outfile:
            # Записываем заголовок с информацией о скрипте
            outfile.write("/*" + "="*70 + "\n")
            outfile.write(f"  Объединенный файл, созданный merge_script.py\n")
            outfile.write(f"  Содержит {len(source_files)} файлов\n")
            outfile.write(f"  Дата создания: {Path(output_file).stat().st_ctime if output_file else 'сейчас'}\n")
            outfile.write("="*70 + " */\n\n")
            
            for idx, file_path in enumerate(source_files, 1):
                # Получаем относительный путь для красоты
                try:
                    rel_path = os.path.relpath(file_path, root_dir)
                except:
                    rel_path = file_path
                
                # Записываем маркер начала файла
                outfile.write(f"\n/*" + "="*70 + "*/\n")
                outfile.write(f"/* Файл {idx}/{len(source_files)}: {rel_path} */\n")
                outfile.write(f"/*" + "="*70 + "*/\n\n")
                
                # Читаем и записываем содержимое файла
                try:
                    with open(file_path, 'r', encoding='utf-8') as infile:
                        content = infile.read()
                        outfile.write(content)
                        
                        # Добавляем перенос строки, если его нет в конце
                        if content and not content.endswith('\n'):
                            outfile.write('\n')
                except UnicodeDecodeError:
                    # Пробуем другие кодировки
                    try:
                        with open(file_path, 'r', encoding='latin-1') as infile:
                            content = infile.read()
                            outfile.write(content)
                            if content and not content.endswith('\n'):
                                outfile.write('\n')
                    except Exception as e:
                        outfile.write(f"// ОШИБКА: Не удалось прочитать файл: {e}\n")
                
                outfile.write("\n\n")  # Отступ между файлами
        
        print(f"✅ Успешно создан файл: {output_file}")
        print(f"📁 Собрано файлов: {len(source_files)}")
        return True
        
    except Exception as e:
        print(f"❌ Ошибка при создании файла: {e}")
        return False

def main():
    # Получаем путь к директории скрипта
    script_dir = Path(__file__).parent.absolute()
    
    # Имя выходного файла
    default_output = "merged_output.txt"
    
    # Проверяем аргументы командной строки
    if len(sys.argv) > 1:
        output_filename = sys.argv[1]
    else:
        output_filename = default_output
    
    # Полный путь к выходному файлу
    output_path = script_dir / output_filename
    
    print(f"🔍 Поиск .cpp и .h файлов в: {script_dir}")
    print(f"📄 Выходной файл: {output_path}")
    print("-" * 50)
    
    # Выполняем объединение
    if merge_files(output_path, script_dir):
        print(f"\n✨ Готово! Файл сохранен как: {output_path}")
    else:
        print("\n❌ Ошибка при выполнении операции")
        sys.exit(1)

if __name__ == "__main__":
    main()