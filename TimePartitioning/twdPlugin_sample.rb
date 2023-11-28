require 'json'

def generate_timewindow_cfg(json_data)
  File.open('timewindow.cfg', 'w') do |file|
    file.puts "DEF_SCY({ #{json_data['SystemCycleTime']} });"
    
    json_data['SOM'].each do |som|
      file.puts "\nCRE_SOM( #{som['name']}, { TA_NULL, #{som['next']} });"
      
      som.each do |key, value|
        next if key == 'name' || key == 'next'
        
        file.puts "CLASS( #{key} ) {"
        value.each_with_index do |(domain, time), index|
          file.puts "\tATT_TWD({ #{domain}, #{som['name']} , #{index + 1}, #{time} });"
        end
        file.puts '}'
      end
    end
  end
end

if ARGV.length != 1
  puts 'Usage: ruby script.rb <path_to_json>'
  exit(1)
end

json_path = ARGV[0]
begin
  json_data = JSON.parse(File.read(json_path))
  generate_timewindow_cfg(json_data)
  puts 'timewindow.cfg generated successfully.'
rescue JSON::ParserError => e
  puts "Error parsing JSON: #{e.message}"
end

# require 'json'

# def generate_ruby_code(json_data)
#   ruby_code = ""

#   ruby_code << "DEF_SCY({ #{json_data['SystemCycleTime']} });\n\n"

#   json_data['SOM'].each do |som|
#     ruby_code << "CRE_SOM( #{som['name']}, { TA_NULL, #{som['Next']} });\n"

#     som.each do |key, value|
#       next if key == 'name' || key == 'Next'

#       ruby_code << "CLASS( #{key} ) {\n"

#       if value.is_a?(Hash)
#         value.each do |attr_key, attr_value|
#           if attr_value.is_a?(Hash) && attr_value.key?('order') && attr_value.key?('value')
#             ruby_code << "\tATT_TWD({ #{attr_key}, #{som['name']} , #{attr_value['order'].to_s}, #{attr_value['value'].to_s} });\n"
#           else
#             puts "Error: Invalid attribute format in SOM '#{som['name']}' and CLASS '#{key}'."
#             exit(1)
#           end
#         end
#       else
#         puts "Error2: Invalid attribute format in SOM '#{som['name']}' and CLASS '#{key}'."
#         exit(1)
#       end


#       ruby_code << "}\n"
#     end

#     ruby_code << "\n"
#   end

#   ruby_code
# end

# def read_json_file(file_path)
#   begin
#     json_data = JSON.parse(File.read(file_path))
#     return json_data
#   rescue => e
#     puts "Error reading JSON file: #{e.message}"
#     exit(1)
#   end
# end

# if ARGV.length != 1
#   puts "Usage: ruby script.rb <json_file>"
#   exit(1)
# end

# json_file_path = ARGV[0]

# # Read JSON data from the specified file
# json_data = read_json_file(json_file_path)

# # Generate Ruby code
# ruby_code = generate_ruby_code(json_data)

# # Write the generated Ruby code to "time_window.cfg" file
# File.open("time_window.cfg", "w") do |file|
#   file.puts(ruby_code)
# end

# puts "Generated Ruby code has been written to 'time_window.cfg'."
