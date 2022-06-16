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

public class MountOptionBuilder {

    private StringBuilder sb = new StringBuilder();

    public MountOptionBuilder() {
    }

    public MountOptionBuilder(MountOption... mountOptions) {
        for (MountOption mountOption : mountOptions) {
            add(mountOption);
        }
    }

    public MountOptionBuilder add(MountOption mountOption) {
        switch (mountOption) {
            case RW -> sb.append("rw").append(",");
            case NODEV -> sb.append("nodev").append(",");
            case NOEXEC -> sb.append("noexec").append(",");
            case NOSUID -> sb.append("nosuid").append(",");
            case RELATIME -> sb.append("relatime").append(",");
            case DEFAULTS -> sb.append("defaults").append(",");
        }
        return this;
    }

    public String create() {
        return sb.deleteCharAt(sb.length() - 1).toString();
    }
}
