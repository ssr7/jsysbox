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

package ir.moke.jsysbox.time;

import ir.moke.jsysbox.JSysboxException;
import ir.moke.jsysbox.JniNativeLoader;

import java.sql.Timestamp;
import java.time.LocalDateTime;
import java.time.ZoneId;
import java.time.ZonedDateTime;
import java.time.temporal.ChronoUnit;

public class JDateTime {
    static {
        JniNativeLoader.load("jdatetime.so");
    }

    public native static void setTimezone(String timezone) throws JSysboxException;

    private native static void setDateTime(long timestamp) throws JSysboxException;
    public native static ZonedDateTime getZonedDateTime() throws JSysboxException;

    public native static void syncSystemToHardware() throws JSysboxException;

    public native static void syncHardwareToSystem() throws JSysboxException;

    public static void setDateTime(LocalDateTime dateTime) throws JSysboxException {
        dateTime = dateTime.truncatedTo(ChronoUnit.SECONDS);
        Timestamp timestamp = Timestamp.valueOf(dateTime.truncatedTo(ChronoUnit.SECONDS));
        setDateTime(timestamp.getTime() / 1000);
    }

    public static void setTimezone(ZoneId zone) throws JSysboxException {
        setTimezone(zone.getId());
    }
}
