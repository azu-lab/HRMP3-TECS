require 'json'

# 入力ファイルパスをコマンドライン引数から取得
input_file_path = ARGV[0]

# 出力先のJSONファイル名
output_file_path = 'twd.json'

# 出力用のハッシュ
output_hash = {
  "SystemCycleTime": 10000,
  "SOM": [
    {
      "name": "SOM1",
      "Next": "SOM1"
    }
  ]
}

# 入力ファイルの内容を読み込み
input_content = File.read(input_file_path)

# 正規表現を使用して必要な情報を抽出
current_class = nil
input_content.each_line do |line|
  if line =~ /\[domain\(HRMP, "user"\)\]\s*region\s*(\w+)/
    current_class = nil
    region_name = $1
  elsif line =~ /\[class\(HRMP, "([^"]+)"\)\]/
    current_class = $1
    output_hash[:SOM][0][current_class] ||= {}
  elsif current_class && line =~ /region\s*(\w+)/
    output_hash[:SOM][0][current_class][$1] ||= 1000
  end
end

# 出力ファイルにJSONを書き込み
File.open(output_file_path, 'w') do |file|
  file.write(JSON.pretty_generate(output_hash))
end

puts "JSONファイルが生成されました: #{output_file_path}"
