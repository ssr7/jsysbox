/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package ir.moke.jsysbox;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Optional;

public class JniNativeLoader {
    private static final Path LIB_PATH = Paths.get("/META-INF/native");

    private static Path TEMP_DIR_PATH = Paths.get("/tmp/jni");

    static {
        try {
            if (!Files.exists(TEMP_DIR_PATH)) {
                Files.createDirectory(TEMP_DIR_PATH);
            }
            String envPath = System.getProperty("java.tmp.path");
            if (envPath != null && !envPath.isEmpty()) TEMP_DIR_PATH = Paths.get(envPath);

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static synchronized void load(String name) {
        extractLibrary(name).ifPresent(library -> System.load(library.toAbsolutePath().toString()));
    }

    private static Optional<Path> extractLibrary(String name) {
        String libAbsolutePath = LIB_PATH.resolve(name).toAbsolutePath().toString();
        try (InputStream resource = JniNativeLoader.class.getResourceAsStream(libAbsolutePath)) {
            if (resource != null) {
                try (FileOutputStream outputStream = new FileOutputStream(TEMP_DIR_PATH.resolve(name).toFile())) {
                    resource.transferTo(outputStream);
                    return Optional.of(TEMP_DIR_PATH.resolve(name));
                }
            } else {
                System.out.println("Resource is null");
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
        return Optional.empty();
    }
}
